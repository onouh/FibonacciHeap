#include "FibonacciHeap.hpp"
#include "Vector.hpp"
#ifndef FIBONACCI_HEAP_TPP
#define FIBONACCI_HEAP_TPP

#include <iostream>
#include <cmath>
using namespace std;

// constructor
template <typename T>
FibonacciHeap<T>::FibonacciHeap() : minNode(nullptr), size(0) {}

// destructor
template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    deleteAll(minNode);
}

// insertBefore()
template <typename T>
void FibonacciHeap<T>::insertBefore(Node* node, Node* target) {
    if (!node || !target) return;
    node->right = target;
    node->left = target->left;
    target->left->right = node;
    target->left = node;
}

// deleteAll() 
template <typename T>
void FibonacciHeap<T>::deleteAll(Node* start) {
    if (!start) return;
    Node* curr = start;
    do {
        Node* next = curr->right;
        if (curr->child)
            deleteAll(curr->child);
        delete curr;
        curr = next;
    } while (curr != start);
}

// insert
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::insert(const T& value, int key) {
    Node* node = new Node(value, key);
    if (!minNode) {
        minNode = node;
    } else {
        insertBefore(node, minNode);
        if (node->key < minNode->key)
            minNode = node;
    }
    size++;
    return node;
}

// getMin()
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::getMin() const { return minNode; }

// isEmpty()
template <typename T>
bool FibonacciHeap<T>::isEmpty() const { return minNode == nullptr; }

// getSize
template <typename T>
int FibonacciHeap<T>::getSize() const { return size; }

// displayMin
template <typename T>
void FibonacciHeap<T>::displayMin() const {
    if (isEmpty()) {
        cout << "Heap is empty\n";
    } else {
        cout << "Min Node Value: " << minNode->value
             << " (Priority = " << minNode->key << ")\n";
    }
}

// merge()
template <typename T>
void FibonacciHeap<T>::merge(FibonacciHeap& otherHeap) {
    if (this == &otherHeap || !otherHeap.minNode) return;
    if (!minNode) {
        minNode = otherHeap.minNode;
    } else {
        Node* thisTemp = minNode->right;
        Node* otherTemp = otherHeap.minNode->left;
        thisTemp->left = otherTemp;
        otherTemp->right = thisTemp;
        minNode->right = otherHeap.minNode;
        otherHeap.minNode->left = minNode;
    }
    if (minNode->key > otherHeap.minNode->key) minNode = otherHeap.minNode;
    size += otherHeap.size;
    otherHeap.minNode = nullptr;
    otherHeap.size = 0;
}

// linkNodes()
template <typename T>
void FibonacciHeap<T>::linkNodes(Node* a, Node* b) {
    if (!a || !b) return;
    b->left->right = b->right;
    b->right->left = b->left;
    if (!a->child) {
        a->child = b;
        b->left = b;
        b->right = b;
    } else {
        insertBefore(b, a->child);
    }
    b->parent = a;
    a->degree++;
    b->marked = false;
}

// consolidate
template <typename T>
void FibonacciHeap<T>::consolidate() {
    if (!minNode) return;

    int maxDegree = static_cast<int>(log2(size) / log2(1.618)) + 1;
    Vector<Node*> degreeArray(maxDegree + 1, nullptr);
    Vector<Node*> rootlist;

    Node* curr = minNode;
    Node* start = minNode;
    do {
        rootlist.push_back(curr);
        curr = curr->right;
    } while (curr != start);

    for (Node* r : rootlist) {
        Node* p = r;
        int d = r->degree;
        while (d < (int)degreeArray.size() && degreeArray[d] != nullptr) {
            Node* c = degreeArray[d];
            if (p->key > c->key) {
                Node* temp = p;
                p = c;
                c = temp;
            }
            linkNodes(p, c);
            degreeArray[d] = nullptr;
            d++;
            if (d >= (int)degreeArray.size()) degreeArray.resize(d + 1, nullptr);
        }
        degreeArray[d] = p;
    }

    minNode = nullptr;
    for (Node* node : degreeArray) {
        if (node) {
            if (!minNode) {
                minNode = node;
                node->right = node;
                node->left = node;
            } else {
                insertBefore(node, minNode);
            }
            if (node->key < minNode->key) minNode = node;
        }
    }
}

// extractMin
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::extractMin() {
    Node* temp = minNode;
    if (!temp) return temp;
    if (temp->child) {
        Node* start = temp->child;
        Node* curr = start;
        do {
            Node* nextChild = curr->right;
            curr->parent = nullptr;
            insertBefore(curr, temp);
            curr = nextChild;
        } while (curr != start);
    }

    temp->left->right = temp->right;
    temp->right->left = temp->left;

    if (temp->right == temp) {
        minNode = nullptr;
    } else {
        minNode = temp->right;
        consolidate();
    }

    size--;
    return temp;
}

// getRootList
template <typename T>
Vector<typename FibonacciHeap<T>::Node*> FibonacciHeap<T>::getRootList() const {
    Vector<Node*> roots;
    if (!minNode) return roots;
    Node* curr = minNode;
    Node* start = minNode;
    do {
        roots.push_back(curr);
        curr = curr->right;
    } while (curr != start);
    return roots;
}

template <typename T>
void FibonacciHeap<T>::cut(Node* x, Node* y) {
    if (x->right == x) {
        y->child = nullptr;
    } else {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) {
            y->child = x->right;
        }
    }
    y->degree--;
    insertBefore(x, minNode);
    x->parent = nullptr;
    x->marked = false;
}

template <typename T>
void FibonacciHeap<T>::cascadingCut(Node* y) {
    Node* z = y->parent;
    if (z) {
        if (!y->marked) {
            y->marked = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename T>
void FibonacciHeap<T>::decreaseKey(Node* x, int newKey) {
    if (newKey > x->key) {
        throw std::invalid_argument("New key is greater than current key");
    }
    x->key = newKey;
    Node* y = x->parent;
    if (y && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
    }
    if (x->key < minNode->key) {
        minNode = x;
    }
}

// findNode()
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::findNode(Node* start, const T& value) {
    if (!start) return nullptr;
    Node* curr = start;
    do {
        if (curr->value == value) return curr;
        Node* foundInChild = findNode(curr->child, value);
        if (foundInChild) return foundInChild;
        curr = curr->right;
    } while (curr != start);
    return nullptr;
}

//search()
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::search(const T& value) {
    return findNode(minNode, value);
}

//deleteNode()
template <typename T>
void FibonacciHeap<T>::deleteNode(Node* x) {
    if (!x) return;
    // it's brought to top by giving it the smallest possible key
    decreaseKey(x, -999999); 
    Node* extracted = extractMin();
    if (extracted) delete extracted;
}

//increaseKey()
template <typename T>
void FibonacciHeap<T>::increaseKey(Node* x, int newKey) {
    if (x == nullptr) return;
    
    // store the value before deleting the node
    T val = x->value;
    
    // the node is removed from its current position
    deleteNode(x); 
    
    // here it's re-inserted with the new key
    // this ensures the heap property is perfectly maintained
    insert(val, newKey);
}

//updateKey() - general method to update key (decides whether to increase or decrease)
template <typename T>
void FibonacciHeap<T>::updateKey(Node* x, int newKey) {
    if (x == nullptr) return;

    if (newKey < x->key) {
        // condition worsened (urgency increases)
        decreaseKey(x, newKey);
    } 
    else if (newKey > x->key) {
        // condition improved (urgency decreases)
        increaseKey(x, newKey);
    }
    // if keys are equal, we do nothing.
}

#endif // FIBONACCI_HEAP_TPP