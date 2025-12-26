#ifndef FIBONACCI_HEAP_TPP
#define FIBONACCI_HEAP_TPP

#include "FibonacciHeap.hpp"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

//constructor
template <typename T>
FibonacciHeap<T>::FibonacciHeap() : minNode(nullptr), size(0) {}

//destructor
template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    deleteAll(minNode);
}

//insertBefore()
template <typename T>
void FibonacciHeap<T>::insertBefore(Node* node, Node* target) {
    if (!node || !target) return;
    node->right = target;
    node->left = target->left;
    target->left->right = node;
    target->left = node;
}

//deleteAll() 
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

//insert
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

//getMin()
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::getMin() const { return minNode; }

//isEmpty()
template <typename T>
bool FibonacciHeap<T>::isEmpty() const { return minNode == nullptr; }

//getSize
template <typename T>
int FibonacciHeap<T>::getSize() const { return size; }

//displayMin
template <typename T>
void FibonacciHeap<T>::displayMin() const {
    if (isEmpty()) {
        cout << "Heap is empty\n";
    } else {
        cout << "Min Node Value: " << minNode->value
                  << " (Priority = " << minNode->key << ")\n";
    }
}

//merge()
template <typename T>
void FibonacciHeap<T>::merge(FibonacciHeap& otherHeap){
    if(this == &otherHeap || !otherHeap.minNode) return;
    if(!minNode) minNode = otherHeap.minNode;
    
    else{
        //split the two circular lists
        Node* thisTemp = minNode->right;
        Node* otherTemp = otherHeap.minNode->left;
        //connect the 2 chains
        thisTemp->left = otherTemp;
        otherTemp->right = thisTemp;
        //connect the 2 minNodes
        minNode->right = otherHeap.minNode;
        otherHeap.minNode->left = minNode;  
    }
    if(minNode->key > otherHeap.minNode->key) minNode = otherHeap.minNode;
    size+= otherHeap.size;
    otherHeap.minNode = nullptr;
    otherHeap.size = 0;
}

//linkNodes()
//this function simply links two nodes, assuming same degrees,
//a->priority (parent) < b->priority (child), //opp!!
//minNode change are handled outside (in consolidate)
template <typename T>
void FibonacciHeap<T>:: linkNodes(Node*a, Node*b){
    if(!a || !b) return;
    //here b is disconnected from the node list
    b->left->right = b->right;
    b->right->left = b->left;
    //here b is inserted as a child of a

    //case: a has no children
    if(!a->child){  
        a->child = b;
        b->left = b;
        b->right = b;
    }
    //case: a has children  
    else{insertBefore(b, a->child);}
    b->parent = a;
    a->degree++;
    b->marked = false; 
}

//consolidate
template <typename T>
void FibonacciHeap<T>::consolidate(){
    if(!minNode) return;
    int maxDegree = static_cast<int>(log2(size) / log2(1.618)) + 1;
    vector<Node*> degreeArray(maxDegree + 1, nullptr);
    //we store the roots in the rootlist
    vector<Node*> rootlist;
    Node*curr=minNode;
    Node* start=minNode;
    do{
        rootlist.push_back(curr);
        curr=curr->right;

    }while(curr!=start);
    //trees with the same degree are merged
    for(Node* r : rootlist){
        Node* p = r;
        int d = r->degree; 
        while(d < degreeArray.size() && degreeArray[d] != nullptr){
            Node* c = degreeArray[d];
            //swap if p's value > c's to ensure p is always the parent
            if(p->key > c->key){
               Node* temp = p;
               p = c;
               c = temp;
            }
            linkNodes(p,c);
            degreeArray[d] = nullptr; //c's previous place
            d++;
            if(d >= degreeArray.size()) degreeArray.resize(d+1,nullptr); //allocate extra space if necessary
        }
        degreeArray[d] = p;
    }
   
    //rebuild the rootlist, start by minNode
    minNode = nullptr;
    for(Node* node : degreeArray){
        if(node){
            if(!minNode){
                minNode = node;
                node -> right = node;
                node -> left = node;
            }
            else{insertBefore(node, minNode);}
            if(node->key < minNode->key) minNode = node; // change minNode if necessary
             
        }
    }
}

// extractMin
// is it better if it returns both bool, node?
template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::extractMin(){
    Node* temp = minNode;
    if(!temp) return temp;
    if(temp->child){
        Node* start = temp->child;
        Node* curr = start;
        do{
            Node *newChild = curr->right;
            curr->parent = nullptr;
            insertBefore(curr, temp);
            curr = newChild;
        }while(curr != start);
    }

    // remove minNode from the rootlist    
    temp->left->right = temp->right;
    temp->right->left = temp->left;

    if(temp->right == temp) minNode = nullptr;
    else{
        minNode = temp->right;
        consolidate();
    }

    size--;
    return temp;
}

// getRootList: return all roots in the circular root list
template <typename T>
std::vector<typename FibonacciHeap<T>::Node*> FibonacciHeap<T>::getRootList() const {
    std::vector<Node*> roots;
    if (!minNode) return roots;
    Node* curr = minNode;
    Node* start = minNode;
    do {
        roots.push_back(curr);
        curr = curr->right;
    } while (curr != start);
    return roots;
}

template<typename T>
void FibonacciHeap<T>::cut(FibonacciHeap<T>::Node* x, FibonacciHeap<T>::Node* y) {
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
void FibonacciHeap<T>::cascadingCut(FibonacciHeap<T>::Node* y) {
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
void FibonacciHeap<T>::decreaseKey(FibonacciHeap<T>::Node* x, T newKey) {
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


#endif