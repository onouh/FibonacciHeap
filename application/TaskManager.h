#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include "FibonacciHeap.hpp"
#include <string>

class TaskManager
{
private:
    FibonacciHeap<std::string> heap;

public:
    template <typename Urgency>
    void addPatient(const std::string &name, Urgency priority);
    template <typename Urgency>
    bool updatePatientStatus(const std::string &name, Urgency newLevel);
    std::string getNextUrgent();
    std::string treatNext();
    int getPendingCount();
};
#endif // TASKMANAGER_HPP