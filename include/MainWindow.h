#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <map>
#include "FibonacciHeap.h"

/**
 * Canvas widget for drawing the Fibonacci Heap
 */
class HeapCanvas : public QWidget {
    Q_OBJECT
    
private:
    FibonacciHeap<int>* heap;
    std::map<FibonacciNode<int>*, QPointF> nodePositions;
    
    // Constants
    static constexpr float NODE_RADIUS = 25.0f;
    static constexpr float HORIZONTAL_SPACING = 100.0f;
    static constexpr float VERTICAL_SPACING = 80.0f;
    
    void calculateNodePositions();
    void positionSubtree(FibonacciNode<int>* node, float x, float y, float& maxX);
    void drawNode(QPainter& painter, FibonacciNode<int>* node, float x, float y, bool isRoot);
    void drawConnections(QPainter& painter);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
public:
    explicit HeapCanvas(FibonacciHeap<int>* h, QWidget* parent = nullptr);
};

/**
 * Main window class for Fibonacci Heap visualization
 * Handles rendering and user interaction
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    FibonacciHeap<int> heap;
    
    // UI Elements
    QLineEdit* inputField;
    QPushButton* insertButton;
    QPushButton* extractMinButton;
    QPushButton* resetButton;
    QLabel* statusLabel;
    HeapCanvas* canvas;
    
    void setupUI();
    void updateStatus(const QString& message);
    
private slots:
    void onInsertClicked();
    void onExtractMinClicked();
    void onResetClicked();
    
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
