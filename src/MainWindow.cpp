#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <cmath>
#include <algorithm>

// UI Constants
namespace {
    constexpr int TITLE_FONT_SIZE = 18;
    constexpr int EMPTY_HEAP_FONT_SIZE = 16;
    constexpr int NODE_TEXT_FONT_SIZE = 12;
}

// HeapCanvas Implementation

/**
 * Constructor for HeapCanvas
 * @param h Pointer to the FibonacciHeap to visualize
 * @param parent Parent widget (optional)
 * 
 * Sets minimum size to 1200x600 to ensure adequate space for heap visualization.
 * The canvas uses an expanding size policy to fill available space.
 */
HeapCanvas::HeapCanvas(FibonacciHeap<int>* h, QWidget* parent)
    : QWidget(parent), heap(h) {
    setMinimumSize(1200, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void HeapCanvas::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Background
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    if (heap->isEmpty()) {
        painter.setPen(Qt::gray);
        QFont font = painter.font();
        font.setPointSize(EMPTY_HEAP_FONT_SIZE);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "Heap is empty");
        return;
    }
    
    calculateNodePositions();
    drawConnections(painter);
    
    // Draw nodes
    for (const auto& [node, pos] : nodePositions) {
        drawNode(painter, node, pos.x(), pos.y(), node->parent == nullptr);
    }
}

void HeapCanvas::calculateNodePositions() {
    nodePositions.clear();
    
    if (heap->isEmpty()) return;
    
    std::vector<FibonacciHeap<int>::Node*> roots = heap->getRootList();
    
    float startX = 100;
    float startY = 100;
    float currentX = startX;
    
    for (auto root : roots) {
        float maxX = currentX;
        positionSubtree(root, currentX, startY, maxX);
        currentX = maxX + HORIZONTAL_SPACING * 1.5f;
    }
}

void HeapCanvas::positionSubtree(FibonacciHeap<int>::Node* node, float x, float y, float& maxX) {
    if (!node) return;
    
    nodePositions[node] = QPointF(x, y);
    maxX = std::max(maxX, x);
    
    // Position children
    if (node->child) {
        FibonacciHeap<int>::Node* child = node->child;
        FibonacciHeap<int>::Node* start = child;
        float childX = x - (node->degree - 1) * HORIZONTAL_SPACING / 2.0f;
        
        do {
            positionSubtree(child, childX, y + VERTICAL_SPACING, maxX);
            childX += HORIZONTAL_SPACING;
            child = child->right;
        } while (child != start);
    }
}

void HeapCanvas::drawConnections(QPainter& painter) {
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    
    for (const auto& [node, pos] : nodePositions) {
        if (node->child) {
            // Draw lines to children
            FibonacciHeap<int>::Node* child = node->child;
            FibonacciHeap<int>::Node* start = child;
            do {
                FibonacciHeap<int>::Node* next = child->right;
                if (nodePositions.find(child) != nodePositions.end()) {
                    QPointF childPos = nodePositions[child];
                    painter.drawLine(pos, childPos);
                }
                child = next;
            } while (child != start);
        }
    }
}

void HeapCanvas::drawNode(QPainter& painter, FibonacciHeap<int>::Node* node, float x, float y, bool isRoot) {
    // Determine node color
    QColor fillColor;
    if (node == heap->getMin()) {
        fillColor = QColor(255, 215, 0); // Gold for minimum
    } else if (node->marked) {
        fillColor = QColor(255, 140, 0); // Orange for marked
    } else if (isRoot) {
        fillColor = QColor(144, 238, 144); // Light green for roots
    } else {
        fillColor = QColor(173, 216, 230); // Light blue for other nodes
    }
    
    // Draw circle
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(fillColor));
    painter.drawEllipse(QPointF(x, y), NODE_RADIUS, NODE_RADIUS);
    
    // Draw key value
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(NODE_TEXT_FONT_SIZE);
    font.setBold(true);
    painter.setFont(font);
    
    QString text = QString::number(node->key);
    QRectF textRect(x - NODE_RADIUS, y - NODE_RADIUS, NODE_RADIUS * 2, NODE_RADIUS * 2);
    painter.drawText(textRect, Qt::AlignCenter, text);
}

// MainWindow Implementation
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setWindowTitle("Fibonacci Heap Visualization");
    resize(1200, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Title label
    QLabel* titleLabel = new QLabel("Fibonacci Heap Visualization", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(TITLE_FONT_SIZE);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    // Instructions label
    QLabel* instructionsLabel = new QLabel(
        "Enter a number and click Insert. Click Extract Min to remove minimum.", this);
    mainLayout->addWidget(instructionsLabel);
    
    // Status label
    statusLabel = new QLabel("Ready", this);
    statusLabel->setStyleSheet("QLabel { color: green; }");
    mainLayout->addWidget(statusLabel);
    
    // Control panel
    QHBoxLayout* controlLayout = new QHBoxLayout();
    
    // Input field
    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Enter number");
    inputField->setMaxLength(10);
    inputField->setFixedWidth(150);
    controlLayout->addWidget(inputField);
    
    // Insert button
    insertButton = new QPushButton("Insert", this);
    insertButton->setStyleSheet("QPushButton { background-color: #4682B4; color: white; padding: 8px 16px; }");
    controlLayout->addWidget(insertButton);
    
    // Extract Min button
    extractMinButton = new QPushButton("Extract Min", this);
    extractMinButton->setStyleSheet("QPushButton { background-color: #DC143C; color: white; padding: 8px 16px; }");
    controlLayout->addWidget(extractMinButton);
    
    // Reset button
    resetButton = new QPushButton("Reset", this);
    resetButton->setStyleSheet("QPushButton { background-color: #808080; color: white; padding: 8px 16px; }");
    controlLayout->addWidget(resetButton);
    
    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);
    
    // Canvas for heap visualization
    canvas = new HeapCanvas(&heap, this);
    mainLayout->addWidget(canvas);
    
    // Connect signals and slots
    connect(insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(extractMinButton, &QPushButton::clicked, this, &MainWindow::onExtractMinClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(inputField, &QLineEdit::returnPressed, this, &MainWindow::onInsertClicked);
}

void MainWindow::onInsertClicked() {
    QString text = inputField->text().trimmed();
    
    if (text.isEmpty()) {
        updateStatus("Please enter a value");
        return;
    }
    
    bool ok;
    int value = text.toInt(&ok);
    
    if (!ok) {
        updateStatus("Invalid input");
        return;
    }
    
    heap.insert(value, value);  // Use value as both the value and key
    updateStatus(QString("Inserted: %1").arg(value));
    inputField->clear();
    canvas->update();
}

void MainWindow::onExtractMinClicked() {
    if (heap.isEmpty()) {
        updateStatus("Heap is empty");
        return;
    }
    
    try {
        FibonacciHeap<int>::Node* minNode = heap.getMin();
        if (!minNode) {
            updateStatus("Error: Heap min node is null");
            return;
        }
        int minValue = minNode->key;
        FibonacciHeap<int>::Node* extracted = heap.extractMin();
        delete extracted;  // Free the extracted node
        updateStatus(QString("Extracted min: %1").arg(minValue));
        canvas->update();
    }
    catch (const std::exception& e) {
        updateStatus("Error extracting min");
    }
}

void MainWindow::onResetClicked() {
    heap = FibonacciHeap<int>();
    updateStatus("Heap reset");
    canvas->update();
}

void MainWindow::updateStatus(const QString& message) {
    statusLabel->setText(message);
}
