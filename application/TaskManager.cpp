#include "TaskManager.h"

template <typename Urgency>
void TaskManager::addPatient(const std::string& name, Urgency urgency) {
    heap.insert(name, static_cast<int>(urgency));
}

template <typename Urgency>
bool TaskManager::updatePatientStatus(const std::string& name, Urgency newLevel) {
    auto* node = heap.search(name);
    if (node) {
        heap.updateKey(node, static_cast<int>(newLevel));
        return true;
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
    delete min;
    return value;
}

int TaskManager::getPendingCount() {
    return heap.getSize();
}