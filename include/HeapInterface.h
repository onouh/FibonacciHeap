#ifndef HEAP_INTERFACE_H
#define HEAP_INTERFACE_H

#include <QString>
#include <vector>
#include <memory>

/**
 * Abstract interface for Fibonacci Heap operations
 * This allows the GUI to work with heaps of different types
 */
class IHeapNode {
public:
    virtual ~IHeapNode() = default;
    virtual QString getDisplayValue() const = 0;
    virtual int getKey() const = 0;
    virtual int getDegree() const = 0;
    virtual bool isMarked() const = 0;
    virtual IHeapNode* getParent() const = 0;
    virtual IHeapNode* getChild() const = 0;
    virtual IHeapNode* getLeft() const = 0;
    virtual IHeapNode* getRight() const = 0;
};

class IFibonacciHeap {
public:
    virtual ~IFibonacciHeap() = default;
    
    // Core operations
    virtual IHeapNode* insert(const QString& value, int key) = 0;
    virtual IHeapNode* getMin() const = 0;
    virtual bool isEmpty() const = 0;
    virtual int getSize() const = 0;
    virtual IHeapNode* extractMin() = 0;
    virtual void decreaseKey(IHeapNode* node, int newKey) = 0;
    virtual void deleteNode(IHeapNode* node) = 0;
    virtual void merge(IFibonacciHeap* other) = 0;
    
    // For visualization
    virtual std::vector<IHeapNode*> getRootList() const = 0;
    virtual IHeapNode* search(const QString& value) = 0;
};

#endif // HEAP_INTERFACE_H
