#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include <memory>
#include <vector>
#include <functional>

/**
 * Node structure for Fibonacci Heap
 */
template<typename T>
struct FibonacciNode {
    T key;
    int degree;
    bool marked;
    FibonacciNode* parent;
    FibonacciNode* child;
    FibonacciNode* left;
    FibonacciNode* right;
    
    FibonacciNode(T k) : key(k), degree(0), marked(false), 
                         parent(nullptr), child(nullptr), 
                         left(this), right(this) {}
};

/**
 * Fibonacci Heap implementation
 * A collection of min-heap-ordered trees following Fibonacci Heap properties
 */
template<typename T>
class FibonacciHeap {
private:
    FibonacciNode<T>* minNode;
    int numNodes;
    
    // Helper functions
    void link(FibonacciNode<T>* y, FibonacciNode<T>* x);
    void consolidate();
    void cut(FibonacciNode<T>* x, FibonacciNode<T>* y);
    void cascadingCut(FibonacciNode<T>* y);
    void addToRootList(FibonacciNode<T>* node);
    void removeFromRootList(FibonacciNode<T>* node);
    void destroyNode(FibonacciNode<T>* node);
    
public:
    FibonacciHeap();
    ~FibonacciHeap();
    
    // Core operations
    FibonacciNode<T>* insert(T key);
    T getMin() const;
    FibonacciNode<T>* extractMin();
    void decreaseKey(FibonacciNode<T>* x, T newKey);
    void deleteNode(FibonacciNode<T>* x);
    void merge(FibonacciHeap<T>& other);
    
    // Utility functions
    bool isEmpty() const { return minNode == nullptr; }
    int size() const { return numNodes; }
    
    // For visualization
    FibonacciNode<T>* getMinNode() const { return minNode; }
    std::vector<FibonacciNode<T>*> getRootList() const;
    void traverse(FibonacciNode<T>* node, std::function<void(FibonacciNode<T>*)> callback) const;
};

#include "FibonacciHeap.tpp"

#endif // FIBONACCIHEAP_H
