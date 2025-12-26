#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QSpinBox>
#include <QSlider>
#include <QTimer>
#include <QMouseEvent>
#include <map>
#include "FibonacciHeap.hpp"
#include "AnimationSystem.h"

/**
 * Canvas widget for drawing the Fibonacci Heap with animation and interaction support
 */
class HeapCanvas : public QWidget {
    Q_OBJECT
    
private:
    FibonacciHeap<int>* heap;
    AnimationSystem* animationSystem;
    std::map<FibonacciHeap<int>::Node*, QPointF> nodePositions;
    FibonacciHeap<int>::Node* selectedNode;
    FibonacciHeap<int>::Node* highlightedNode;
    
    // Constants - Increased spacing to prevent node overlap
    static constexpr float NODE_RADIUS = 30.0f;
    static constexpr float HORIZONTAL_SPACING = 150.0f;
    static constexpr float VERTICAL_SPACING = 120.0f;
    static constexpr float SIBLING_SPACING = 100.0f;
    
    void calculateNodePositions();
    void positionSubtree(FibonacciHeap<int>::Node* node, float x, float y, float& maxX, int depth = 0);
    void drawNode(QPainter& painter, FibonacciHeap<int>::Node* node, float x, float y, bool isRoot);
    void drawConnections(QPainter& painter);
    void drawPointerLines(QPainter& painter);
    FibonacciHeap<int>::Node* findNodeAtPosition(const QPointF& pos);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    
public:
    explicit HeapCanvas(FibonacciHeap<int>* h, AnimationSystem* anim, QWidget* parent = nullptr);
    
    FibonacciHeap<int>::Node* getSelectedNode() const { return selectedNode; }
    void setHighlightedNode(FibonacciHeap<int>::Node* node);
    void clearSelection();
    
signals:
    void nodeSelected(FibonacciHeap<int>::Node* node);
};

/**
 * Main window class for enhanced Fibonacci Heap visualization
 * Supports animations and all heap operations
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    FibonacciHeap<int> heap;
    FibonacciHeap<int> heap2;  // For union operation
    AnimationSystem* animationSystem;
    
    // UI Elements - Input
    QLineEdit* inputField;
    QSpinBox* keySpinBox;
    
    // UI Elements - Buttons
    QPushButton* insertButton;
    QPushButton* findMinButton;
    QPushButton* extractMinButton;
    QPushButton* decreaseKeyButton;
    QPushButton* deleteNodeButton;
    QPushButton* unionButton;
    QPushButton* resetButton;
    
    // Animation controls
    QSlider* speedSlider;
    QPushButton* pauseResumeButton;
    
    // Display elements
    QLabel* statusLabel;
    QLabel* infoLabel;
    HeapCanvas* canvas;
    
    void setupUI();
    void updateStatus(const QString& message);
    void updateInfo();
    void setupAnimationForExtractMin();
    void setupAnimationForDecreaseKey();
    
private slots:
    void onInsertClicked();
    void onFindMinClicked();
    void onExtractMinClicked();
    void onDecreaseKeyClicked();
    void onDeleteNodeClicked();
    void onUnionClicked();
    void onResetClicked();
    void onPauseResumeClicked();
    void onSpeedChanged(int value);
    void onAnimationUpdate();
    void onAnimationCompleted();
    void onNodeSelected(FibonacciHeap<int>::Node* node);
    
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
