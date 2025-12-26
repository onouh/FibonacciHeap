#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include "FibonacciHeap.hpp"
#include <string>
#include <vector>

// Urgency levels for emergency care management
enum class Urgency {
    CRITICAL = 1,    // Life-threatening, immediate attention
    URGENT = 2,      // Serious, needs prompt care
    MODERATE = 3,    // Stable but needs attention
    MINOR = 4        // Non-urgent
};

// Structure to hold task information
struct Task {
    std::string name;
    Urgency urgency;
    FibonacciHeap<std::string>::Node* heapNode;
    
    Task(const std::string& n, Urgency u, FibonacciHeap<std::string>::Node* node)
        : name(n), urgency(u), heapNode(node) {}
};

class TaskManager
{
private:
    FibonacciHeap<std::string> heap;
    std::vector<Task> tasks;  // Keep track of all tasks

public:
    void addPatient(const std::string &name, Urgency priority);
    bool updatePatientStatus(const std::string &name, Urgency newLevel);
    std::string getNextUrgent();
    std::string treatNext();
    int getPendingCount();
    std::vector<Task> getAllTasks() const;
    void removeTask(const std::string& name);
};
#endif // TASKMANAGER_HPP