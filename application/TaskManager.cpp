#include "TaskManager.h"
#include <algorithm>

void TaskManager::addPatient(const std::string& name, Urgency urgency) {
    auto* node = heap.insert(name, static_cast<int>(urgency));
    tasks.push_back(Task(name, urgency, node));
}

bool TaskManager::updatePatientStatus(const std::string& name, Urgency newLevel) {
    // Find task in our list
    for (auto& task : tasks) {
        if (task.name == name && task.heapNode) {
            int newPriority = static_cast<int>(newLevel);
            int currentPriority = static_cast<int>(task.urgency);
            
            // Only use decreaseKey if new priority is better (lower number = higher priority)
            if (newPriority < currentPriority) {
                heap.decreaseKey(task.heapNode, newPriority);
                task.urgency = newLevel;
                return true;
            } else if (newPriority > currentPriority) {
                // For increasing priority (lower urgency), we need to delete and re-insert
                heap.deleteNode(task.heapNode);
                task.heapNode = heap.insert(name, newPriority);
                task.urgency = newLevel;
                return true;
            }
            // If same priority, no change needed
            return true;
        }
    }
    return false;
}

std::string TaskManager::getNextUrgent() {
    if (heap.isEmpty()) return "";
    return heap.getMin()->value;
}

std::string TaskManager::treatNext() {
    if (heap.isEmpty()) return "";
    auto* min = heap.extractMin();
    std::string value = min->value;
    
    // Remove from tasks list - O(n) operation
    // Note: Using vector for simplicity. For larger datasets, consider using 
    // std::unordered_map<std::string, Task*> for O(1) lookup and removal
    tasks.erase(
        std::remove_if(tasks.begin(), tasks.end(),
            [&value](const Task& t) { return t.name == value; }),
        tasks.end()
    );
    
    delete min;
    return value;
}

int TaskManager::getPendingCount() {
    return heap.getSize();
}

std::vector<Task> TaskManager::getAllTasks() const {
    return tasks;
}

void TaskManager::removeTask(const std::string& name) {
    // Find and remove from tasks list
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [&name](const Task& t) { return t.name == name; });
    
    if (it != tasks.end()) {
        if (it->heapNode) {
            heap.deleteNode(it->heapNode);
        }
        tasks.erase(it);
    }
}