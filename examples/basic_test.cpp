#include "../include/FibonacciHeap.h"
#include <iostream>
#include <vector>

void printHeapInfo(const FibonacciHeap<int>& heap) {
    if (heap.isEmpty()) {
        std::cout << "Heap is empty" << std::endl;
    } else {
        std::cout << "Heap size: " << heap.size() 
                  << ", Min: " << heap.getMin() << std::endl;
    }
}

int main() {
    std::cout << "=== Fibonacci Heap Test ===" << std::endl << std::endl;
    
    FibonacciHeap<int> heap;
    
    // Test 1: Insert operations
    std::cout << "Test 1: Inserting values 10, 5, 20, 3, 15" << std::endl;
    heap.insert(10);
    heap.insert(5);
    heap.insert(20);
    heap.insert(3);
    heap.insert(15);
    printHeapInfo(heap);
    std::cout << "Expected: size=5, min=3" << std::endl << std::endl;
    
    // Test 2: Extract min
    std::cout << "Test 2: Extract minimum" << std::endl;
    auto minNode = heap.extractMin();
    std::cout << "Extracted: " << minNode->key << std::endl;
    delete minNode;
    printHeapInfo(heap);
    std::cout << "Expected: extracted=3, size=4, min=5" << std::endl << std::endl;
    
    // Test 3: More inserts
    std::cout << "Test 3: Insert 2 and 8" << std::endl;
    heap.insert(2);
    heap.insert(8);
    printHeapInfo(heap);
    std::cout << "Expected: size=6, min=2" << std::endl << std::endl;
    
    // Test 4: Extract all elements
    std::cout << "Test 4: Extract all elements in order" << std::endl;
    std::cout << "Extracted values: ";
    while (!heap.isEmpty()) {
        auto node = heap.extractMin();
        std::cout << node->key << " ";
        delete node;
    }
    std::cout << std::endl;
    std::cout << "Expected order: 2 5 8 10 15 20" << std::endl << std::endl;
    
    // Test 5: Merge operation
    std::cout << "Test 5: Merge two heaps" << std::endl;
    FibonacciHeap<int> heap1;
    FibonacciHeap<int> heap2;
    
    heap1.insert(4);
    heap1.insert(7);
    heap1.insert(9);
    
    heap2.insert(1);
    heap2.insert(6);
    heap2.insert(11);
    
    std::cout << "Heap1: ";
    printHeapInfo(heap1);
    std::cout << "Heap2: ";
    printHeapInfo(heap2);
    
    heap1.merge(heap2);
    std::cout << "After merge: ";
    printHeapInfo(heap1);
    std::cout << "Expected: size=6, min=1" << std::endl << std::endl;
    
    // Test 6: Decrease key
    std::cout << "Test 6: Decrease key operation" << std::endl;
    FibonacciHeap<int> heap3;
    auto node1 = heap3.insert(10);
    auto node2 = heap3.insert(20);
    auto node3 = heap3.insert(30);
    
    std::cout << "Before decrease key: ";
    printHeapInfo(heap3);
    
    heap3.decreaseKey(node3, 5);
    std::cout << "After decreasing 30 to 5: ";
    printHeapInfo(heap3);
    std::cout << "Expected: size=3, min=5" << std::endl << std::endl;
    
    std::cout << "=== All tests completed ===" << std::endl;
    
    return 0;
}
