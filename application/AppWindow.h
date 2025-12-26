#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QScrollArea>
#include <QComboBox>
#include <QStatusBar>
#include "TaskManager.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

private:
    TaskManager taskManager;
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QVBoxLayout* taskListLayout;
    QScrollArea* scrollArea;
    QLabel *timeLabel, *activeTasksLabel;
    QTimer* timer;
    int taskCounter;

    void setupUI();
    void initializeSampleTasks();
    void refreshTaskList();
    int getActiveTaskCount();
    QString urgencyToString(Urgency priority) const;
    QColor urgencyToColor(Urgency priority) const;

private slots:
    void addTask();
    void viewUrgent();
    void treatNext();
    void completeTask(const QString& taskName);
    void updateTaskPriority(const QString& taskName);
    void updateTime();
    void showNotification(const QString& message);

public:
    explicit AppWindow(QWidget* parent = nullptr);
    ~AppWindow();
};
#endif // APPWINDOW_H