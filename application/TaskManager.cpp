#include "FibonacciHeap.hpp"
#include <string>
#include <iostream>

using namespace std;

//urgency enum
//placed here for global access
enum Urgency {
    CRITICAL = 1,
    URGENT = 3,
    MODERATE = 5,
    STABLE = 8,
    ROUTINE = 10
};

inline const char* getUrgencyLabel(int key) {
    if (key <= 1) return "CRITICAL";
    if (key <= 3) return "URGENT";
    if (key <= 5) return "MODERATE";
    if (key <= 8) return "STABLE";
    return "ROUTINE";
}

class TaskManager {
    private:
        FibonacciHeap<string> heap;

    public:
    
        //addPatient() - inserts a new patient with a given urgency level
        void addPatient(string name, Urgency priority) {
            heap.insert(name, static_cast<int>(priority));
        }

        //updatePatientStatus() - updates a patient's urgency level
        bool updatePatientStatus(string name, Urgency newLevel) {
            FibonacciHeap<string>::Node* patientNode = heap.search(name);
            
            if (patientNode == nullptr) return false;
            heap.updateKey(patientNode, static_cast<int>(newLevel));
            return true;
        }

        //getNextUrgent() - shows the next patient to be treated without removing them
        string getNextUrgent() {
            if (heap.isEmpty()) return "";
            return heap.getMin()->value;
        }

        //treatNext() - extracts and returns the highest priority patient
        string treatNext() {
            if (heap.isEmpty()) return "";
            FibonacciHeap<string>::Node* min = heap.extractMin();
            string name = min->value;
            delete min;
            return name;
        }

        //getPendingCount() - returns the number of patients waiting
        int getPendingCount() {
            return heap.getSize();
        }
};