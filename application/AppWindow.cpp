#include "AppWindow.h"
#include "TaskManager.h"
#include <QDateTime>
#include <QGroupBox>
#include <QFrame>

AppWindow::AppWindow(QWidget* parent) 
    : QMainWindow(parent), taskCounter(0) {
    setupUI();
    initializeSampleTasks();
    refreshTaskList();
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AppWindow::updateTime);
    timer->start(1000);
    updateTime();
}

AppWindow::~AppWindow() {
    // Qt handles memory cleanup via parent-child hierarchy
}

void AppWindow::setupUI() {
    setWindowTitle("Emergency Care Management System");
    setMinimumSize(900, 700);
    
    // Central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Header section
    QLabel* titleLabel = new QLabel("🏥 Emergency Care Management", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; padding: 10px; background-color: #ecf0f1; border-radius: 5px;");
    mainLayout->addWidget(titleLabel);
    
    // Status bar with time and active tasks
    QHBoxLayout* statusLayout = new QHBoxLayout();
    timeLabel = new QLabel("Time: --:--:--", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #34495e; padding: 5px;");
    activeTasksLabel = new QLabel("0 Active Tasks", this);
    activeTasksLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e74c3c; padding: 5px;");
    statusLayout->addWidget(timeLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(activeTasksLabel);
    mainLayout->addLayout(statusLayout);
    
    // Control buttons
    QGroupBox* controlGroup = new QGroupBox("Actions", this);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlGroup);
    
    QPushButton* addBtn = new QPushButton("➕ Add Patient", this);
    addBtn->setStyleSheet("background-color: #3498db; color: white; padding: 10px; font-size: 13px; border-radius: 5px;");
    connect(addBtn, &QPushButton::clicked, this, &AppWindow::addTask);
    
    QPushButton* urgentBtn = new QPushButton("⚠️ View Most Urgent", this);
    urgentBtn->setStyleSheet("background-color: #f39c12; color: white; padding: 10px; font-size: 13px; border-radius: 5px;");
    connect(urgentBtn, &QPushButton::clicked, this, &AppWindow::viewUrgent);
    
    QPushButton* treatBtn = new QPushButton("✅ Treat Next", this);
    treatBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 10px; font-size: 13px; border-radius: 5px;");
    connect(treatBtn, &QPushButton::clicked, this, &AppWindow::treatNext);
    
    controlLayout->addWidget(addBtn);
    controlLayout->addWidget(urgentBtn);
    controlLayout->addWidget(treatBtn);
    mainLayout->addWidget(controlGroup);
    
    // Task list area
    QGroupBox* taskGroup = new QGroupBox("Patient Queue (Priority Order)", this);
    QVBoxLayout* taskGroupLayout = new QVBoxLayout(taskGroup);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #bdc3c7; background-color: #ffffff; }");
    
    QWidget* scrollWidget = new QWidget();
    taskListLayout = new QVBoxLayout(scrollWidget);
    taskListLayout->setAlignment(Qt::AlignTop);
    taskListLayout->setSpacing(5);
    scrollArea->setWidget(scrollWidget);
    
    taskGroupLayout->addWidget(scrollArea);
    mainLayout->addWidget(taskGroup);
    
    // Legend
    QLabel* legendLabel = new QLabel(
        "Priority Levels: 🔴 CRITICAL | 🟠 URGENT | 🟡 MODERATE | 🟢 MINOR", this);
    legendLabel->setStyleSheet("font-size: 11px; color: #7f8c8d; padding: 5px;");
    legendLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(legendLabel);
}

void AppWindow::initializeSampleTasks() {
    taskManager.addPatient("Patient A - Cardiac Arrest (Room 201)", Urgency::CRITICAL);
    taskManager.addPatient("Patient B - Severe Bleeding (Room 305)", Urgency::URGENT);
    taskManager.addPatient("Patient C - Broken Arm (Room 108)", Urgency::MODERATE);
    taskCounter = 3;
}

void AppWindow::refreshTaskList() {
    // Clear existing task widgets
    QLayoutItem* item;
    while ((item = taskListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Get all tasks from task manager
    auto tasks = taskManager.getAllTasks();
    
    if (tasks.empty()) {
        QLabel* emptyLabel = new QLabel("No patients in queue", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #95a5a6; font-size: 14px; padding: 20px;");
        taskListLayout->addWidget(emptyLabel);
    } else {
        // Sort tasks by urgency (priority)
        std::sort(tasks.begin(), tasks.end(),
            [](const Task& a, const Task& b) {
                return static_cast<int>(a.urgency) < static_cast<int>(b.urgency);
            });
        
        for (const auto& task : tasks) {
            QFrame* taskFrame = new QFrame(this);
            taskFrame->setStyleSheet(
                "QFrame { background-color: " + urgencyToColor(task.urgency).name() + 
                "; border-left: 5px solid #34495e; border-radius: 3px; padding: 8px; margin: 2px; }"
            );
            
            QHBoxLayout* taskLayout = new QHBoxLayout(taskFrame);
            
            // Priority indicator
            QString urgencyStr = urgencyToString(task.urgency);
            QLabel* priorityLabel = new QLabel(urgencyStr, this);
            priorityLabel->setFixedWidth(100);
            priorityLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
            
            // Task name
            QLabel* nameLabel = new QLabel(QString::fromStdString(task.name), this);
            nameLabel->setStyleSheet("color: #2c3e50; font-size: 12px;");
            
            // Action buttons
            QPushButton* completeBtn = new QPushButton("Complete", this);
            completeBtn->setFixedWidth(80);
            completeBtn->setStyleSheet("background-color: #27ae60; color: white; border-radius: 3px; padding: 5px;");
            connect(completeBtn, &QPushButton::clicked, this, [this, task]() {
                completeTask(QString::fromStdString(task.name));
            });
            
            QPushButton* updateBtn = new QPushButton("Update", this);
            updateBtn->setFixedWidth(80);
            updateBtn->setStyleSheet("background-color: #3498db; color: white; border-radius: 3px; padding: 5px;");
            connect(updateBtn, &QPushButton::clicked, this, [this, task]() {
                updateTaskPriority(QString::fromStdString(task.name));
            });
            
            taskLayout->addWidget(priorityLabel);
            taskLayout->addWidget(nameLabel);
            taskLayout->addStretch();
            taskLayout->addWidget(updateBtn);
            taskLayout->addWidget(completeBtn);
            
            taskListLayout->addWidget(taskFrame);
        }
    }
    
    // Update active count
    activeTasksLabel->setText(QString::number(taskManager.getPendingCount()) + " Active Tasks");
}

void AppWindow::addTask() {
    bool ok;
    QString taskName = QInputDialog::getText(this, "Add Patient", 
        "Enter patient name and location:", QLineEdit::Normal, "", &ok);
    
    if (ok && !taskName.isEmpty()) {
        QStringList urgencyLevels = {"CRITICAL", "URGENT", "MODERATE", "MINOR"};
        QString urgencyStr = QInputDialog::getItem(this, "Select Priority",
            "Choose urgency level:", urgencyLevels, 0, false, &ok);
        
        if (ok) {
            Urgency urgency = Urgency::MODERATE;
            if (urgencyStr == "CRITICAL") urgency = Urgency::CRITICAL;
            else if (urgencyStr == "URGENT") urgency = Urgency::URGENT;
            else if (urgencyStr == "MODERATE") urgency = Urgency::MODERATE;
            else if (urgencyStr == "MINOR") urgency = Urgency::MINOR;
            
            taskManager.addPatient(taskName.toStdString(), urgency);
            taskCounter++;
            showNotification("✅ Patient added: " + taskName);
            refreshTaskList();
        }
    }
}

void AppWindow::viewUrgent() {
    std::string urgent = taskManager.getNextUrgent();
    if (urgent.empty()) {
        showNotification("ℹ️ No patients in queue");
    } else {
        QMessageBox::information(this, "Most Urgent Patient", 
            "Next patient to treat:\n\n" + QString::fromStdString(urgent));
    }
}

void AppWindow::treatNext() {
    std::string treated = taskManager.treatNext();
    if (treated.empty()) {
        showNotification("ℹ️ No patients in queue");
    } else {
        showNotification("✅ Treated: " + QString::fromStdString(treated));
        refreshTaskList();
    }
}

void AppWindow::completeTask(const QString& taskName) {
    taskManager.removeTask(taskName.toStdString());
    showNotification("✅ Completed: " + taskName);
    refreshTaskList();
}

void AppWindow::updateTaskPriority(const QString& taskName) {
    QStringList urgencyLevels = {"CRITICAL", "URGENT", "MODERATE", "MINOR"};
    bool ok;
    QString urgencyStr = QInputDialog::getItem(this, "Update Priority",
        "Choose new urgency level for:\n" + taskName, urgencyLevels, 0, false, &ok);
    
    if (ok) {
        Urgency urgency = Urgency::MODERATE;
        if (urgencyStr == "CRITICAL") urgency = Urgency::CRITICAL;
        else if (urgencyStr == "URGENT") urgency = Urgency::URGENT;
        else if (urgencyStr == "MODERATE") urgency = Urgency::MODERATE;
        else if (urgencyStr == "MINOR") urgency = Urgency::MINOR;
        
        if (taskManager.updatePatientStatus(taskName.toStdString(), urgency)) {
            showNotification("✅ Priority updated for: " + taskName);
            refreshTaskList();
        } else {
            showNotification("❌ Failed to update priority");
        }
    }
}

void AppWindow::updateTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    timeLabel->setText("🕐 " + currentTime.toString("hh:mm:ss"));
}

void AppWindow::showNotification(const QString& message) {
    statusBar()->showMessage(message, 3000);
}

QString AppWindow::urgencyToString(Urgency priority) const {
    switch (priority) {
        case Urgency::CRITICAL: return "🔴 CRITICAL";
        case Urgency::URGENT: return "🟠 URGENT";
        case Urgency::MODERATE: return "🟡 MODERATE";
        case Urgency::MINOR: return "🟢 MINOR";
        default: return "UNKNOWN";
    }
}

QColor AppWindow::urgencyToColor(Urgency priority) const {
    switch (priority) {
        case Urgency::CRITICAL: return QColor("#ffebee");  // Light red
        case Urgency::URGENT: return QColor("#fff3e0");    // Light orange
        case Urgency::MODERATE: return QColor("#fff9c4");  // Light yellow
        case Urgency::MINOR: return QColor("#e8f5e9");     // Light green
        default: return QColor("#f5f5f5");
    }
}