#include "MainWindow.h"
#include "TaskManager.h" // Include TaskManager
#include <QDateTime>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent), taskCounter(0) {
    setupUI();
    initializeSampleTasks();
    refreshTaskList();
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);
    updateTime();
}

MainWindow::~MainWindow() {
    // GUI task cleanup
}

void MainWindow::setupUI() {
    setWindowTitle("Emergency Care Management v2.0");
    setMinimumSize(1400, 800);
    ...
}

void MainWindow::initializeSampleTasks() {
    taskManager.addPatient("Patient A (Room 201)", Urgency::CRITICAL);
    taskManager.addPatient("Patient B (Room 305)", Urgency::URGENT);
    taskCounter += 2;
    refreshTaskList();
}

void MainWindow::refreshTaskList() {
    // Clear and re-render GUI tasks list
    ...
    // Fetch sorted tasks from TaskManager
    QString nextUrgentName = QString::fromStdString(taskManager.getNextUrgent());
    for (/* Loop through taskManager's heap (TaskManager API) */) {
        ...
    }
    activeTasksLabel->setText(QString::number(taskManager.getPendingCount()) + " Active");
}

void MainWindow::addTask() {
    bool ok;
    QString taskName = QInputDialog::getText(this, "New Task", ...);
    // Validations + Add via TaskManager
    refreshTaskList();
}

void MainWindow::viewUrgent() {
    ...
    QString urgent = QString::fromStdString(taskManager.getNextUrgent());
    ...
}

void MainWindow::treatNext() {
    std::string treated = taskManager.treatNext(); // Handle via TaskManager
    refreshTaskList();
}

void MainWindow::updateTime() {
    // Dynamic Time GUI Effect
    ...
}