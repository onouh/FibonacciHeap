#include "../include/FibonacciHeap.h"
#include <iostream>
#include <vector>
#include <random>

/**
 * Advanced example demonstrating Fibonacci Heap usage
 * including decrease key and delete operations
 */

void printSeparator() {
    std::cout << "\n" << std::string(60, '=') << "\n" << std::endl;
}

void demonstrateDecreaseKey() {
    std::cout << "DECREASE KEY DEMONSTRATION" << std::endl;
    std::cout << "Creating heap with values: 50, 40, 30, 20, 10" << std::endl;
    
    FibonacciHeap<int> heap;
    auto node50 = heap.insert(50);
    auto node40 = heap.insert(40);
    auto node30 = heap.insert(30);
    auto node20 = heap.insert(20);
    auto node10 = heap.insert(10);
    
    std::cout << "Initial minimum: " << heap.getMin() << std::endl;
    
    // Decrease key of 50 to 5
    std::cout << "Decreasing key 50 -> 5" << std::endl;
    heap.decreaseKey(node50, 5);
    std::cout << "New minimum: " << heap.getMin() << std::endl;
    
    // Extract all to show order
    std::cout << "Extracting all values: ";
    while (!heap.isEmpty()) {
        auto node = heap.extractMin();
        std::cout << node->key << " ";
        delete node;
    }
    std::cout << std::endl;
}

void demonstrateDeleteNode() {
    std::cout << "DELETE NODE DEMONSTRATION" << std::endl;
    std::cout << "Creating heap with values: 15, 25, 35, 45, 55" << std::endl;
    
    FibonacciHeap<int> heap;
    heap.insert(15);
    auto node25 = heap.insert(25);
    heap.insert(35);
    heap.insert(45);
    heap.insert(55);
    
    std::cout << "Initial size: " << heap.size() << std::endl;
    std::cout << "Deleting node with value 25" << std::endl;
    
    heap.deleteNode(node25);
    
    std::cout << "New size: " << heap.size() << std::endl;
    std::cout << "Remaining values: ";
    while (!heap.isEmpty()) {
        auto node = heap.extractMin();
        std::cout << node->key << " ";
        delete node;
    }
    std::cout << std::endl;
}

void demonstrateMerge() {
    std::cout << "MERGE DEMONSTRATION" << std::endl;
    
    FibonacciHeap<int> heap1;
    FibonacciHeap<int> heap2;
    
    std::cout << "Heap 1: inserting even numbers (2, 4, 6, 8)" << std::endl;
    heap1.insert(2);
    heap1.insert(4);
    heap1.insert(6);
    heap1.insert(8);
    
    std::cout << "Heap 2: inserting odd numbers (1, 3, 5, 7)" << std::endl;
    heap2.insert(1);
    heap2.insert(3);
    heap2.insert(5);
    heap2.insert(7);
    
    std::cout << "Heap 1 min: " << heap1.getMin() << ", size: " << heap1.size() << std::endl;
    std::cout << "Heap 2 min: " << heap2.getMin() << ", size: " << heap2.size() << std::endl;
    
    std::cout << "Merging heap2 into heap1..." << std::endl;
    heap1.merge(heap2);
    
    std::cout << "After merge - min: " << heap1.getMin() << ", size: " << heap1.size() << std::endl;
    std::cout << "Heap2 is now empty: " << (heap2.isEmpty() ? "true" : "false") << std::endl;
    
    std::cout << "Merged heap values: ";
    while (!heap1.isEmpty()) {
        auto node = heap1.extractMin();
        std::cout << node->key << " ";
        delete node;
    }
    std::cout << std::endl;
}

void performanceTest() {
    std::cout << "PERFORMANCE TEST" << std::endl;
    
    const int NUM_OPERATIONS = 1000;
    FibonacciHeap<int> heap;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    
    std::cout << "Inserting " << NUM_OPERATIONS << " random values..." << std::endl;
    std::vector<FibonacciNode<int>*> nodes;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        nodes.push_back(heap.insert(dis(gen)));
    }
    
    std::cout << "Heap size: " << heap.size() << std::endl;
    std::cout << "Minimum value: " << heap.getMin() << std::endl;
    
    std::cout << "Performing 100 decrease key operations..." << std::endl;
    for (int i = 0; i < 100 && i < nodes.size(); i++) {
        try {
            heap.decreaseKey(nodes[i], nodes[i]->key - 1);
        } catch (...) {
            // Skip if decrease would increase key
        }
    }
    
    std::cout << "New minimum after decrease keys: " << heap.getMin() << std::endl;
    
    std::cout << "Extracting 50 minimum values: ";
    for (int i = 0; i < 50 && !heap.isEmpty(); i++) {
        auto node = heap.extractMin();
        if (i < 10) {
            std::cout << node->key << " ";
        } else if (i == 10) {
            std::cout << "... ";
        }
        delete node;
    }
    std::cout << std::endl;
    std::cout << "Remaining heap size: " << heap.size() << std::endl;
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     FIBONACCI HEAP - ADVANCED EXAMPLES                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    printSeparator();
    demonstrateDecreaseKey();
    
    printSeparator();
    demonstrateDeleteNode();
    
    printSeparator();
    demonstrateMerge();
    
    printSeparator();
    performanceTest();
    
    printSeparator();
    std::cout << "All demonstrations completed successfully!" << std::endl;
    std::cout << "\n";
    
    return 0;
}
