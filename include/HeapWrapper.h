#ifndef HEAP_WRAPPER_H
#define HEAP_WRAPPER_H

#include "HeapInterface.h"
#include "FibonacciHeap.hpp"
#include <QString>
#include <string>
#include <sstream>

// Template wrapper for FibonacciHeap nodes
template<typename T>
class HeapNodeWrapper : public IHeapNode {
private:
    typename FibonacciHeap<T>::Node* node;
    
public:
    explicit HeapNodeWrapper(typename FibonacciHeap<T>::Node* n) : node(n) {}
    
    typename FibonacciHeap<T>::Node* getActualNode() const { return node; }
    
    QString getDisplayValue() const override {
        if (!node) return "";
        std::ostringstream oss;
        oss << node->value;
        return QString::fromStdString(oss.str());
    }
    
    int getKey() const override { return node ? node->key : 0; }
    int getDegree() const override { return node ? node->degree : 0; }
    bool isMarked() const override { return node ? node->marked : false; }
    
    IHeapNode* getParent() const override {
        if (!node || !node->parent) return nullptr;
        return new HeapNodeWrapper<T>(node->parent);
    }
    
    IHeapNode* getChild() const override {
        if (!node || !node->child) return nullptr;
        return new HeapNodeWrapper<T>(node->child);
    }
    
    IHeapNode* getLeft() const override {
        if (!node || !node->left) return nullptr;
        return new HeapNodeWrapper<T>(node->left);
    }
    
    IHeapNode* getRight() const override {
        if (!node || !node->right) return nullptr;
        return new HeapNodeWrapper<T>(node->right);
    }
};

// Template wrapper for FibonacciHeap
template<typename T>
class HeapWrapper : public IFibonacciHeap {
private:
    FibonacciHeap<T> heap;
    
    T parseValue(const QString& str) {
        if constexpr (std::is_same_v<T, int>) {
            return str.toInt();
        } else if constexpr (std::is_same_v<T, char>) {
            return str.isEmpty() ? '\0' : str.at(0).toLatin1();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return str.toStdString();
        }
        return T();
    }
    
public:
    HeapWrapper() = default;
    ~HeapWrapper() override = default;
    
    IHeapNode* insert(const QString& value, int key) override {
        T val = parseValue(value);
        auto* node = heap.insert(val, key);
        return new HeapNodeWrapper<T>(node);
    }
    
    IHeapNode* getMin() const override {
        auto* node = heap.getMin();
        return node ? new HeapNodeWrapper<T>(node) : nullptr;
    }
    
    bool isEmpty() const override {
        return heap.isEmpty();
    }
    
    int getSize() const override {
        return heap.getSize();
    }
    
    IHeapNode* extractMin() override {
        auto* node = heap.extractMin();
        if (!node) return nullptr;
        auto* wrapper = new HeapNodeWrapper<T>(node);
        delete node;  // Clean up the actual node
        return wrapper;
    }
    
    void decreaseKey(IHeapNode* node, int newKey) override {
        auto* wrapper = dynamic_cast<HeapNodeWrapper<T>*>(node);
        if (wrapper && wrapper->getActualNode()) {
            heap.decreaseKey(wrapper->getActualNode(), newKey);
        }
    }
    
    void deleteNode(IHeapNode* node) override {
        auto* wrapper = dynamic_cast<HeapNodeWrapper<T>*>(node);
        if (wrapper && wrapper->getActualNode()) {
            heap.deleteNode(wrapper->getActualNode());
        }
    }
    
    void merge(IFibonacciHeap* other) override {
        auto* otherWrapper = dynamic_cast<HeapWrapper<T>*>(other);
        if (otherWrapper) {
            heap.merge(otherWrapper->heap);
        }
    }
    
    std::vector<IHeapNode*> getRootList() const override {
        Vector<typename FibonacciHeap<T>::Node*> roots = heap.getRootList();
        std::vector<IHeapNode*> result;
        for (size_t i = 0; i < roots.size(); ++i) {
            result.push_back(new HeapNodeWrapper<T>(roots[i]));
        }
        return result;
    }
    
    IHeapNode* search(const QString& value) override {
        T val = parseValue(value);
        auto* node = heap.search(val);
        return node ? new HeapNodeWrapper<T>(node) : nullptr;
    }
    
    FibonacciHeap<T>& getActualHeap() { return heap; }
};

#endif // HEAP_WRAPPER_H
