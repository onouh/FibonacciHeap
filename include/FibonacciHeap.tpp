#ifndef FIBONACCIHEAP_TPP
#define FIBONACCIHEAP_TPP

#include <cmath>
#include <limits>
#include <stdexcept>

template<typename T>
FibonacciHeap<T>::FibonacciHeap() : minNode(nullptr), numNodes(0) {}

template<typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    if (minNode) {
        // Destroy all nodes starting from root list
        std::vector<FibonacciNode<T>*> roots = getRootList();
        for (auto root : roots) {
            destroyNode(root);
        }
    }
}

template<typename T>
void FibonacciHeap<T>::destroyNode(FibonacciNode<T>* node) {
    if (!node) return;
    
    // Recursively destroy children
    if (node->child) {
        FibonacciNode<T>* child = node->child;
        FibonacciNode<T>* start = child;
        do {
            FibonacciNode<T>* next = child->right;
            destroyNode(child);
            child = next;
        } while (child != start);
    }
    
    delete node;
}

template<typename T>
FibonacciNode<T>* FibonacciHeap<T>::insert(T key) {
    FibonacciNode<T>* node = new FibonacciNode<T>(key);
    
    if (!minNode) {
        minNode = node;
    } else {
        // Add to root list
        node->left = minNode;
        node->right = minNode->right;
        minNode->right->left = node;
        minNode->right = node;
        
        // Update min if necessary
        if (key < minNode->key) {
            minNode = node;
        }
    }
    
    numNodes++;
    return node;
}

template<typename T>
T FibonacciHeap<T>::getMin() const {
    if (!minNode) {
        throw std::runtime_error("Heap is empty");
    }
    return minNode->key;
}

template<typename T>
void FibonacciHeap<T>::link(FibonacciNode<T>* y, FibonacciNode<T>* x) {
    // Remove y from root list
    y->left->right = y->right;
    y->right->left = y->left;
    
    // Make y a child of x
    y->parent = x;
    
    if (!x->child) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    
    x->degree++;
    y->marked = false;
}

template<typename T>
void FibonacciHeap<T>::consolidate() {
    if (!minNode) return;
    
    // Calculate max degree
    int maxDegree = static_cast<int>(std::log2(numNodes)) + 1;
    std::vector<FibonacciNode<T>*> degreeTable(maxDegree + 1, nullptr);
    
    // Get all root nodes
    std::vector<FibonacciNode<T>*> rootList;
    FibonacciNode<T>* current = minNode;
    FibonacciNode<T>* start = minNode;
    do {
        rootList.push_back(current);
        current = current->right;
    } while (current != start);
    
    // Process each root
    for (auto w : rootList) {
        FibonacciNode<T>* x = w;
        int d = x->degree;
        
        while (d < degreeTable.size() && degreeTable[d] != nullptr) {
            FibonacciNode<T>* y = degreeTable[d];
            
            if (x->key > y->key) {
                std::swap(x, y);
            }
            
            link(y, x);
            degreeTable[d] = nullptr;
            d++;
            
            // Expand degree table if needed
            if (d >= degreeTable.size()) {
                degreeTable.resize(d + 1, nullptr);
            }
        }
        
        degreeTable[d] = x;
    }
    
    // Reconstruct root list and find new minimum
    minNode = nullptr;
    for (auto node : degreeTable) {
        if (node) {
            if (!minNode) {
                minNode = node;
                node->left = node;
                node->right = node;
            } else {
                // Add to root list
                node->left = minNode;
                node->right = minNode->right;
                minNode->right->left = node;
                minNode->right = node;
                
                if (node->key < minNode->key) {
                    minNode = node;
                }
            }
        }
    }
}

template<typename T>
FibonacciNode<T>* FibonacciHeap<T>::extractMin() {
    FibonacciNode<T>* z = minNode;
    
    if (z) {
        // Add all children of z to root list
        if (z->child) {
            FibonacciNode<T>* child = z->child;
            FibonacciNode<T>* start = child;
            do {
                FibonacciNode<T>* next = child->right;
                
                // Add child to root list
                child->left = minNode;
                child->right = minNode->right;
                minNode->right->left = child;
                minNode->right = child;
                
                child->parent = nullptr;
                child = next;
            } while (child != start);
        }
        
        // Remove z from root list
        z->left->right = z->right;
        z->right->left = z->left;
        
        if (z == z->right) {
            minNode = nullptr;
        } else {
            minNode = z->right;
            consolidate();
        }
        
        numNodes--;
    }
    
    return z;
}

template<typename T>
void FibonacciHeap<T>::cut(FibonacciNode<T>* x, FibonacciNode<T>* y) {
    // Remove x from child list of y
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
    
    // Add x to root list
    x->left = minNode;
    x->right = minNode->right;
    minNode->right->left = x;
    minNode->right = x;
    
    x->parent = nullptr;
    x->marked = false;
}

template<typename T>
void FibonacciHeap<T>::cascadingCut(FibonacciNode<T>* y) {
    FibonacciNode<T>* z = y->parent;
    
    if (z) {
        if (!y->marked) {
            y->marked = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template<typename T>
void FibonacciHeap<T>::decreaseKey(FibonacciNode<T>* x, T newKey) {
    if (newKey > x->key) {
        throw std::invalid_argument("New key is greater than current key");
    }
    
    x->key = newKey;
    FibonacciNode<T>* y = x->parent;
    
    if (y && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
    }
    
    if (x->key < minNode->key) {
        minNode = x;
    }
}

template<typename T>
void FibonacciHeap<T>::deleteNode(FibonacciNode<T>* x) {
    // Decrease key to minimum possible value
    T minValue = std::numeric_limits<T>::lowest();
    
    // Temporarily set to minimum
    x->key = minValue;
    FibonacciNode<T>* y = x->parent;
    
    if (y) {
        cut(x, y);
        cascadingCut(y);
    }
    
    minNode = x;
    
    // Extract the minimum (which is x)
    FibonacciNode<T>* extracted = extractMin();
    delete extracted;
}

template<typename T>
void FibonacciHeap<T>::merge(FibonacciHeap<T>& other) {
    if (!other.minNode) return;
    
    if (!minNode) {
        minNode = other.minNode;
    } else {
        // Concatenate root lists
        FibonacciNode<T>* thisRight = minNode->right;
        FibonacciNode<T>* otherLeft = other.minNode->left;
        
        minNode->right = other.minNode;
        other.minNode->left = minNode;
        thisRight->left = otherLeft;
        otherLeft->right = thisRight;
        
        // Update minimum
        if (other.minNode->key < minNode->key) {
            minNode = other.minNode;
        }
    }
    
    numNodes += other.numNodes;
    
    // Clear the other heap
    other.minNode = nullptr;
    other.numNodes = 0;
}

template<typename T>
std::vector<FibonacciNode<T>*> FibonacciHeap<T>::getRootList() const {
    std::vector<FibonacciNode<T>*> roots;
    
    if (!minNode) return roots;
    
    FibonacciNode<T>* current = minNode;
    do {
        roots.push_back(current);
        current = current->right;
    } while (current != minNode);
    
    return roots;
}

template<typename T>
void FibonacciHeap<T>::traverse(FibonacciNode<T>* node, std::function<void(FibonacciNode<T>*)> callback) const {
    if (!node) return;
    
    FibonacciNode<T>* current = node;
    do {
        callback(current);
        if (current->child) {
            traverse(current->child, callback);
        }
        current = current->right;
    } while (current != node);
}

#endif // FIBONACCIHEAP_TPP
