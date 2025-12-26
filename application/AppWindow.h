#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include "TaskManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    TaskManager taskManager;  // Updated with TaskManager integration
    QVBoxLayout* taskListLayout;
    QLabel *timeLabel, *activeTasksLabel;
    QTimer* timer;
    int taskCounter;

    void setupUI();
    void initializeSampleTasks();
    void refreshTaskList();
    int getActiveTaskCount();
    QString urgencyToString(Urgency priority) const;

private slots:
    void addTask();
    void viewUrgent();
    void treatNext();
    void completeTask(const QString& taskId);
    void updateTaskPriority(const QString& taskId);
    void updateTime();
    void showNotification(const QString& message);

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H