#include "FibonacciHeap.h"
#include <iostream>
#include <vector>

int main() {
    FibonacciHeap<int> heap;
    
    std::cout << "=== Fibonacci Heap Test ===" << std::endl;
    std::cout << std::endl;
    
    // Test 1: Insert operations
    std::cout << "Test 1: Inserting values 10, 5, 20, 3, 15" << std::endl;
    std::vector<int> values = {10, 5, 20, 3, 15};
    for (int val : values) {
        heap.insert(val);
        std::cout << "  Inserted: " << val << ", Min: " << heap.getMin() << std::endl;
    }
    std::cout << "  Heap size: " << heap.size() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Extract min
    std::cout << "Test 2: Extract min operations" << std::endl;
    while (!heap.isEmpty()) {
        int min = heap.getMin();
        auto node = heap.extractMin();
        std::cout << "  Extracted: " << min;
        if (!heap.isEmpty()) {
            std::cout << ", New min: " << heap.getMin();
        }
        std::cout << std::endl;
        delete node;
    }
    std::cout << "  Heap is now empty" << std::endl;
    std::cout << std::endl;
    
    // Test 3: Decrease key
    std::cout << "Test 3: Decrease key operation" << std::endl;
    auto node1 = heap.insert(50);
    auto node2 = heap.insert(30);
    auto node3 = heap.insert(40);
    std::cout << "  Inserted: 50, 30, 40" << std::endl;
    std::cout << "  Min: " << heap.getMin() << std::endl;
    
    heap.decreaseKey(node1, 10);
    std::cout << "  Decreased 50 to 10" << std::endl;
    std::cout << "  New min: " << heap.getMin() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Merge heaps
    std::cout << "Test 4: Merge heaps" << std::endl;
    FibonacciHeap<int> heap2;
    heap2.insert(100);
    heap2.insert(200);
    std::cout << "  Created second heap with: 100, 200" << std::endl;
    std::cout << "  Heap1 min: " << heap.getMin() << ", size: " << heap.size() << std::endl;
    std::cout << "  Heap2 min: " << heap2.getMin() << ", size: " << heap2.size() << std::endl;
    
    heap.merge(heap2);
    std::cout << "  After merge:" << std::endl;
    std::cout << "  Combined heap min: " << heap.getMin() << ", size: " << heap.size() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Delete node
    std::cout << "Test 5: Delete specific node" << std::endl;
    auto nodeToDelete = heap.insert(25);
    std::cout << "  Inserted: 25" << std::endl;
    std::cout << "  Heap size: " << heap.size() << std::endl;
    
    heap.deleteNode(nodeToDelete);
    std::cout << "  Deleted node with value 25" << std::endl;
    std::cout << "  Heap size: " << heap.size() << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== All tests completed successfully! ===" << std::endl;
    
    return 0;
}
