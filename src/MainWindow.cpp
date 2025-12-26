#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QToolTip>
#include <QPainterPath>
#include <cmath>
#include <algorithm>

// UI Constants
namespace {
    constexpr int TITLE_FONT_SIZE = 18;
    constexpr int EMPTY_HEAP_FONT_SIZE = 16;
    constexpr int NODE_TEXT_FONT_SIZE = 11;
}

// =====================================================================
// HeapCanvas Implementation
// =====================================================================

HeapCanvas::HeapCanvas(FibonacciHeap<int>* h, AnimationSystem* anim, QWidget* parent)
    : QWidget(parent), heap(h), animationSystem(anim), 
      selectedNode(nullptr), highlightedNode(nullptr) {
    setMinimumSize(1400, 700);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

void HeapCanvas::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Background
    painter.fillRect(rect(), QColor(250, 250, 250));
    
    if (heap->isEmpty()) {
        painter.setPen(Qt::gray);
        QFont font = painter.font();
        font.setPointSize(EMPTY_HEAP_FONT_SIZE);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "Heap is empty - Click Insert to add nodes");
        return;
    }
    
    calculateNodePositions();
    
    // Draw pointer lines (sibling relationships)
    drawPointerLines(painter);
    
    // Draw parent-child connections
    drawConnections(painter);
    
    // Draw nodes
    for (const auto& [node, pos] : nodePositions) {
        drawNode(painter, node, pos.x(), pos.y(), node->parent == nullptr);
    }
}

void HeapCanvas::calculateNodePositions() {
    nodePositions.clear();
    
    if (heap->isEmpty()) return;
    
    Vector<FibonacciHeap<int>::Node*> roots = heap->getRootList();
    
    float startX = 150;
    float startY = 100;
    float currentX = startX;
    
    for (size_t i = 0; i < roots.size(); ++i) {
        float maxX = currentX;
        positionSubtree(roots[i], currentX, startY, maxX, 0);
        currentX = maxX + HORIZONTAL_SPACING * 1.5f;
    }
}

void HeapCanvas::positionSubtree(FibonacciHeap<int>::Node* node, float x, float y, float& maxX, int depth) {
    if (!node) return;
    
    nodePositions[node] = QPointF(x, y);
    maxX = std::max(maxX, x);
    
    // Position children
    if (node->child) {
        FibonacciHeap<int>::Node* child = node->child;
        FibonacciHeap<int>::Node* start = child;
        
        // Count children
        int childCount = 0;
        FibonacciHeap<int>::Node* temp = child;
        do {
            childCount++;
            temp = temp->right;
        } while (temp != start);
        
        // Calculate spacing
        float totalChildWidth = (childCount - 1) * SIBLING_SPACING;
        float childX = x - totalChildWidth / 2.0f;
        
        do {
            positionSubtree(child, childX, y + VERTICAL_SPACING, maxX, depth + 1);
            childX += SIBLING_SPACING;
            child = child->right;
        } while (child != start);
    }
}

void HeapCanvas::drawConnections(QPainter& painter) {
    QPen pen(QColor(100, 100, 100), 2);
    painter.setPen(pen);
    
    for (const auto& [node, pos] : nodePositions) {
        if (node->child) {
            // Draw lines to children
            FibonacciHeap<int>::Node* child = node->child;
            FibonacciHeap<int>::Node* start = child;
            do {
                if (nodePositions.find(child) != nodePositions.end()) {
                    QPointF childPos = nodePositions[child];
                    painter.drawLine(pos, childPos);
                    
                    // Draw small arrowhead
                    QPointF direction = childPos - pos;
                    float length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
                    if (length > 0) {
                        direction /= length;
                        QPointF arrowTip = childPos - direction * NODE_RADIUS;
                        QPointF perpendicular(-direction.y(), direction.x());
                        painter.drawLine(arrowTip, arrowTip - direction * 8 + perpendicular * 4);
                        painter.drawLine(arrowTip, arrowTip - direction * 8 - perpendicular * 4);
                    }
                }
                child = child->right;
            } while (child != start);
        }
    }
}

void HeapCanvas::drawPointerLines(QPainter& painter) {
    // Draw sibling connections as dashed lines
    QPen siblingPen(QColor(200, 200, 200), 1, Qt::DashLine);
    painter.setPen(siblingPen);
    
    for (const auto& [node, pos] : nodePositions) {
        // Draw connection to right sibling for root nodes
        if (node->parent == nullptr && node->right != node) {
            if (nodePositions.find(node->right) != nodePositions.end()) {
                QPointF rightPos = nodePositions[node->right];
                // Draw curved line above nodes
                QPainterPath path;
                path.moveTo(pos.x() + NODE_RADIUS, pos.y());
                QPointF control((pos.x() + rightPos.x()) / 2, pos.y() - 30);
                path.quadTo(control, QPointF(rightPos.x() - NODE_RADIUS, rightPos.y()));
                painter.drawPath(path);
            }
        }
    }
}

void HeapCanvas::drawNode(QPainter& painter, FibonacciHeap<int>::Node* node, float x, float y, bool isRoot) {
    // Determine node color
    QColor fillColor;
    bool isMin = (node == heap->getMin());
    bool isSelected = (node == selectedNode);
    bool isHighlighted = (node == highlightedNode);
    
    if (isHighlighted) {
        fillColor = QColor(255, 255, 0);  // Yellow for highlighted (Find Min)
    } else if (isMin) {
        fillColor = QColor(255, 215, 0);  // Gold for minimum
    } else if (node->marked) {
        fillColor = QColor(255, 140, 0);  // Orange for marked
    } else if (isRoot) {
        fillColor = QColor(144, 238, 144);  // Light green for roots
    } else {
        fillColor = QColor(173, 216, 230);  // Light blue for other nodes
    }
    
    // Draw selection ring if selected
    if (isSelected) {
        painter.setPen(QPen(Qt::red, 4));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(x, y), NODE_RADIUS + 5, NODE_RADIUS + 5);
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
    
    // Draw degree indicator
    if (node->degree > 0) {
        font.setPointSize(8);
        font.setBold(false);
        painter.setFont(font);
        painter.setPen(QColor(100, 100, 100));
        QString degreeText = QString("d:%1").arg(node->degree);
        QRectF degreeRect(x - NODE_RADIUS, y + NODE_RADIUS - 12, NODE_RADIUS * 2, 10);
        painter.drawText(degreeRect, Qt::AlignCenter, degreeText);
    }
    
    // Draw marked indicator (small red dot)
    if (node->marked) {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawEllipse(QPointF(x + NODE_RADIUS - 8, y - NODE_RADIUS + 8), 4, 4);
    }
}

FibonacciHeap<int>::Node* HeapCanvas::findNodeAtPosition(const QPointF& pos) {
    for (const auto& [node, nodePos] : nodePositions) {
        float dx = pos.x() - nodePos.x();
        float dy = pos.y() - nodePos.y();
        float distanceSquared = dx * dx + dy * dy;
        if (distanceSquared <= NODE_RADIUS * NODE_RADIUS) {
            return node;
        }
    }
    return nullptr;
}

void HeapCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        FibonacciHeap<int>::Node* node = findNodeAtPosition(event->position());
        if (node) {
            selectedNode = node;
            emit nodeSelected(node);
            update();
        }
    }
}

void HeapCanvas::mouseMoveEvent(QMouseEvent* event) {
    FibonacciHeap<int>::Node* node = findNodeAtPosition(event->position());
    if (node) {
        // Show tooltip with node information
        QString tooltip = QString("Key: %1\nValue: %2\nDegree: %3\nMarked: %4")
            .arg(node->key)
            .arg(node->value)
            .arg(node->degree)
            .arg(node->marked ? "Yes" : "No");
        QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void HeapCanvas::setHighlightedNode(FibonacciHeap<int>::Node* node) {
    highlightedNode = node;
    update();
}

void HeapCanvas::clearSelection() {
    selectedNode = nullptr;
    highlightedNode = nullptr;
    update();
}

// =====================================================================
// MainWindow Implementation
// =====================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    animationSystem = new AnimationSystem(this);
    connect(animationSystem, &AnimationSystem::animationUpdate, this, &MainWindow::onAnimationUpdate);
    connect(animationSystem, &AnimationSystem::animationCompleted, this, &MainWindow::onAnimationCompleted);
    
    setupUI();
    setWindowTitle("Fibonacci Heap - Interactive Visualization (Integer Type)");
    resize(1600, 900);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Title label
    QLabel* titleLabel = new QLabel("Fibonacci Heap - Interactive Visualization with Animations", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(TITLE_FONT_SIZE);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Status and info area
    QHBoxLayout* statusLayout = new QHBoxLayout();
    statusLabel = new QLabel("Ready - Insert values to begin", this);
    statusLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    statusLayout->addWidget(statusLabel);
    
    infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignRight);
    updateInfo();
    statusLayout->addWidget(infoLabel);
    mainLayout->addLayout(statusLayout);
    
    // Control panel
    QGroupBox* controlGroup = new QGroupBox("Heap Operations", this);
    QGridLayout* controlLayout = new QGridLayout(controlGroup);
    
    int row = 0;
    
    // Insert controls
    QLabel* insertLabel = new QLabel("Insert Value:", this);
    controlLayout->addWidget(insertLabel, row, 0);
    
    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Enter integer (e.g., 42)");
    inputField->setMaximumWidth(200);
    controlLayout->addWidget(inputField, row, 1);
    
    insertButton = new QPushButton("Insert", this);
    insertButton->setStyleSheet("QPushButton { background-color: #4682B4; color: white; padding: 8px 16px; font-weight: bold; }");
    controlLayout->addWidget(insertButton, row, 2);
    row++;
    
    // Find Min button
    findMinButton = new QPushButton("Find Min (Highlight)", this);
    findMinButton->setStyleSheet("QPushButton { background-color: #FFD700; color: black; padding: 8px 16px; font-weight: bold; }");
    controlLayout->addWidget(findMinButton, row, 0, 1, 3);
    row++;
    
    // Extract Min button
    extractMinButton = new QPushButton("Extract Min (Animated)", this);
    extractMinButton->setStyleSheet("QPushButton { background-color: #DC143C; color: white; padding: 8px 16px; font-weight: bold; }");
    controlLayout->addWidget(extractMinButton, row, 0, 1, 3);
    row++;
    
    // Decrease Key controls
    QLabel* decreaseKeyLabel = new QLabel("Decrease Key To:", this);
    controlLayout->addWidget(decreaseKeyLabel, row, 0);
    
    keySpinBox = new QSpinBox(this);
    keySpinBox->setRange(-10000, 10000);
    keySpinBox->setValue(0);
    keySpinBox->setMaximumWidth(100);
    controlLayout->addWidget(keySpinBox, row, 1);
    
    decreaseKeyButton = new QPushButton("Decrease Key (Animated)", this);
    decreaseKeyButton->setStyleSheet("QPushButton { background-color: #FF8C00; color: white; padding: 8px 16px; font-weight: bold; }");
    decreaseKeyButton->setToolTip("Select a node by clicking on it first");
    controlLayout->addWidget(decreaseKeyButton, row, 2);
    row++;
    
    // Delete Node button
    deleteNodeButton = new QPushButton("Delete Selected Node", this);
    deleteNodeButton->setStyleSheet("QPushButton { background-color: #8B0000; color: white; padding: 8px 16px; font-weight: bold; }");
    deleteNodeButton->setToolTip("Select a node by clicking on it first");
    controlLayout->addWidget(deleteNodeButton, row, 0, 1, 3);
    row++;
    
    // Union button
    unionButton = new QPushButton("Union with New Heap", this);
    unionButton->setStyleSheet("QPushButton { background-color: #9370DB; color: white; padding: 8px 16px; font-weight: bold; }");
    unionButton->setToolTip("Merge current heap with a new heap");
    controlLayout->addWidget(unionButton, row, 0, 1, 3);
    row++;
    
    // Reset button
    resetButton = new QPushButton("Reset Heap", this);
    resetButton->setStyleSheet("QPushButton { background-color: #808080; color: white; padding: 8px 16px; font-weight: bold; }");
    controlLayout->addWidget(resetButton, row, 0, 1, 3);
    row++;
    
    mainLayout->addWidget(controlGroup);
    
    // Animation controls
    QGroupBox* animGroup = new QGroupBox("Animation Controls", this);
    QHBoxLayout* animLayout = new QHBoxLayout(animGroup);
    
    QLabel* speedLabel = new QLabel("Speed:", this);
    animLayout->addWidget(speedLabel);
    
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 3);
    speedSlider->setValue(1);
    speedSlider->setTickPosition(QSlider::TicksBelow);
    speedSlider->setTickInterval(1);
    speedSlider->setMaximumWidth(150);
    animLayout->addWidget(speedSlider);
    
    QLabel* speedInfo = new QLabel("1=Slow  2=Normal  3=Fast", this);
    animLayout->addWidget(speedInfo);
    
    pauseResumeButton = new QPushButton("Pause", this);
    pauseResumeButton->setEnabled(false);
    pauseResumeButton->setStyleSheet("QPushButton { padding: 8px 16px; }");
    animLayout->addWidget(pauseResumeButton);
    
    animLayout->addStretch();
    mainLayout->addWidget(animGroup);
    
    // Canvas for heap visualization
    canvas = new HeapCanvas(&heap, animationSystem, this);
    mainLayout->addWidget(canvas, 1);  // Stretch factor 1
    
    // Legend
    QGroupBox* legendGroup = new QGroupBox("Legend", this);
    QHBoxLayout* legendLayout = new QHBoxLayout(legendGroup);
    
    auto addLegendItem = [&](const QString& text, const QColor& color) {
        QLabel* colorLabel = new QLabel(this);
        colorLabel->setFixedSize(20, 20);
        colorLabel->setStyleSheet(QString("QLabel { background-color: %1; border: 2px solid black; }").arg(color.name()));
        legendLayout->addWidget(colorLabel);
        QLabel* textLabel = new QLabel(text, this);
        legendLayout->addWidget(textLabel);
    };
    
    addLegendItem("Minimum", QColor(255, 215, 0));
    addLegendItem("Root", QColor(144, 238, 144));
    addLegendItem("Marked", QColor(255, 140, 0));
    addLegendItem("Regular", QColor(173, 216, 230));
    addLegendItem("Highlighted", QColor(255, 255, 0));
    addLegendItem("Selected", QColor(255, 0, 0));
    legendLayout->addStretch();
    
    mainLayout->addWidget(legendGroup);
    
    // Connect signals and slots
    connect(insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(findMinButton, &QPushButton::clicked, this, &MainWindow::onFindMinClicked);
    connect(extractMinButton, &QPushButton::clicked, this, &MainWindow::onExtractMinClicked);
    connect(decreaseKeyButton, &QPushButton::clicked, this, &MainWindow::onDecreaseKeyClicked);
    connect(deleteNodeButton, &QPushButton::clicked, this, &MainWindow::onDeleteNodeClicked);
    connect(unionButton, &QPushButton::clicked, this, &MainWindow::onUnionClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(pauseResumeButton, &QPushButton::clicked, this, &MainWindow::onPauseResumeClicked);
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(inputField, &QLineEdit::returnPressed, this, &MainWindow::onInsertClicked);
    connect(canvas, &HeapCanvas::nodeSelected, this, &MainWindow::onNodeSelected);
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
        updateStatus("Invalid input - please enter an integer");
        return;
    }
    
    heap.insert(value, value);  // Use value as both value and key
    updateStatus(QString("Inserted: %1").arg(value));
    inputField->clear();
    updateInfo();
    canvas->update();
}

void MainWindow::onFindMinClicked() {
    if (heap.isEmpty()) {
        updateStatus("Heap is empty");
        return;
    }
    
    auto* minNode = heap.getMin();
    if (minNode) {
        canvas->setHighlightedNode(minNode);
        updateStatus(QString("Minimum value: %1 (highlighted in yellow)").arg(minNode->key));
    }
}

void MainWindow::onExtractMinClicked() {
    if (heap.isEmpty()) {
        updateStatus("Heap is empty");
        return;
    }
    
    if (animationSystem->isAnimating()) {
        updateStatus("Animation in progress. Please wait...");
        return;
    }
    
    auto* minNode = heap.getMin();
    if (!minNode) {
        updateStatus("Error: Heap min node is null");
        return;
    }
    
    int minValue = minNode->key;
    
    // Setup animation
    setupAnimationForExtractMin();
    
    // Perform actual extraction
    auto* extracted = heap.extractMin();
    delete extracted;
    
    // Start animation
    animationSystem->play();
    pauseResumeButton->setEnabled(true);
    
    updateStatus(QString("Extracting min: %1 - Watch the animation!").arg(minValue));
    updateInfo();
}

void MainWindow::onDecreaseKeyClicked() {
    auto* selectedNode = canvas->getSelectedNode();
    if (!selectedNode) {
        updateStatus("Please select a node first by clicking on it");
        return;
    }
    
    if (animationSystem->isAnimating()) {
        updateStatus("Animation in progress. Please wait...");
        return;
    }
    
    int newKey = keySpinBox->value();
    
    if (newKey >= selectedNode->key) {
        updateStatus(QString("New key (%1) must be less than current key (%2)")
            .arg(newKey).arg(selectedNode->key));
        return;
    }
    
    // Setup animation
    setupAnimationForDecreaseKey();
    
    // Perform actual decrease key
    try {
        heap.decreaseKey(selectedNode, newKey);
        animationSystem->play();
        pauseResumeButton->setEnabled(true);
        updateStatus(QString("Decreasing key to %1 - Watch for cuts and cascading cuts!").arg(newKey));
        updateInfo();
    } catch (const std::exception& e) {
        updateStatus(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onDeleteNodeClicked() {
    auto* selectedNode = canvas->getSelectedNode();
    if (!selectedNode) {
        updateStatus("Please select a node first by clicking on it");
        return;
    }
    
    if (animationSystem->isAnimating()) {
        updateStatus("Animation in progress. Please wait...");
        return;
    }
    
    int key = selectedNode->key;
    
    heap.deleteNode(selectedNode);
    canvas->clearSelection();
    updateStatus(QString("Deleted node with key %1").arg(key));
    updateInfo();
    canvas->update();
}

void MainWindow::onUnionClicked() {
    if (animationSystem->isAnimating()) {
        updateStatus("Animation in progress. Please wait...");
        return;
    }
    
    // Create a new heap with some values
    heap2 = FibonacciHeap<int>();
    heap2.insert(100, 100);
    heap2.insert(50, 50);
    heap2.insert(75, 75);
    
    int heap2Size = heap2.getSize();
    heap.merge(heap2);
    
    updateStatus(QString("Merged with new heap containing %1 nodes (50, 75, 100)").arg(heap2Size));
    updateInfo();
    canvas->update();
}

void MainWindow::onResetClicked() {
    if (animationSystem->isAnimating()) {
        animationSystem->stop();
        pauseResumeButton->setEnabled(false);
    }
    
    heap = FibonacciHeap<int>();
    canvas->clearSelection();
    updateStatus("Heap reset - Ready to insert new values");
    updateInfo();
    canvas->update();
}

void MainWindow::onPauseResumeClicked() {
    if (animationSystem->isAnimating()) {
        animationSystem->pause();
        pauseResumeButton->setText("Resume");
    } else {
        animationSystem->resume();
        pauseResumeButton->setText("Pause");
    }
}

void MainWindow::onSpeedChanged(int value) {
    animationSystem->setSpeed(value);
    updateStatus(QString("Animation speed set to %1").arg(value));
}

void MainWindow::onAnimationUpdate() {
    canvas->update();
}

void MainWindow::onAnimationCompleted() {
    pauseResumeButton->setEnabled(false);
    pauseResumeButton->setText("Pause");
    canvas->update();
    updateStatus("Animation completed");
}

void MainWindow::onNodeSelected(FibonacciHeap<int>::Node* node) {
    if (node) {
        updateStatus(QString("Node selected: key=%1, degree=%2, marked=%3")
            .arg(node->key)
            .arg(node->degree)
            .arg(node->marked ? "Yes" : "No"));
    }
}

void MainWindow::updateStatus(const QString& message) {
    statusLabel->setText(message);
}

void MainWindow::updateInfo() {
    QString info = QString("Heap Size: %1").arg(heap.getSize());
    if (!heap.isEmpty()) {
        info += QString(" | Min Key: %1").arg(heap.getMin()->key);
    }
    infoLabel->setText(info);
}

void MainWindow::setupAnimationForExtractMin() {
    animationSystem->clearSteps();
    
    // Add animation steps for extract min operation
    // Step 1: Highlight the min node
    AnimationStep step1;
    step1.type = AnimationStep::HIGHLIGHT_NODE;
    step1.nodePtr = heap.getMin();
    step1.duration = 500;
    animationSystem->addStep(step1);
    
    // Step 2: Show removal
    AnimationStep step2;
    step2.type = AnimationStep::REMOVE_NODE;
    step2.nodePtr = heap.getMin();
    step2.duration = 800;
    animationSystem->addStep(step2);
    
    // Step 3: Pause to show consolidation
    AnimationStep step3;
    step3.type = AnimationStep::PAUSE;
    step3.duration = 1000;
    animationSystem->addStep(step3);
}

void MainWindow::setupAnimationForDecreaseKey() {
    animationSystem->clearSteps();
    
    auto* node = canvas->getSelectedNode();
    if (!node) return;
    
    // Add animation steps for decrease key operation
    // Step 1: Highlight the node
    AnimationStep step1;
    step1.type = AnimationStep::HIGHLIGHT_NODE;
    step1.nodePtr = node;
    step1.duration = 500;
    animationSystem->addStep(step1);
    
    // Step 2: Show color change (marking)
    AnimationStep step2;
    step2.type = AnimationStep::CHANGE_COLOR;
    step2.nodePtr = node;
    step2.startColor = QColor(173, 216, 230);
    step2.endColor = QColor(255, 140, 0);
    step2.duration = 800;
    animationSystem->addStep(step2);
    
    // Step 3: Pause to show cut
    AnimationStep step3;
    step3.type = AnimationStep::PAUSE;
    step3.duration = 1000;
    animationSystem->addStep(step3);
}
