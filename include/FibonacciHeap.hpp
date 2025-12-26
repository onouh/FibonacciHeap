#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

template <typename T>
class FibonacciHeap {
// nested Node class    
public:

    class Node {
        public:
        T value;         
        int key;    
        int degree;
        bool marked;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        Node(const T& val, int p)
            : value(val), key(p), degree(0), marked(false),
              parent(nullptr), child(nullptr), left(this), right(this) {}
    };

private:

    Node* minNode;
    int size; 

    void insertBefore(Node* node, Node* target);
    void deleteAll(Node* start);

public:

    FibonacciHeap();
    ~FibonacciHeap();

    Node* insert(const T& value, int key);
    Node* getMin() const;
    bool isEmpty() const;
    int getSize() const;
    void displayMin() const;
    void cut(Node* x, Node* y);
    void cascadingCut(Node* y);
    void decreaseKey(Node* x, T newKey);
    void merge(FibonacciHeap& otherHeap);
    void linkNodes(Node*a, Node*b);
    void consolidate();
    Node* extractMin();
    std::vector<Node*> getRootList() const;
 
};

#include "FibonacciHeap.tpp"

#endif // FIBONACCI_HEAP_HPP