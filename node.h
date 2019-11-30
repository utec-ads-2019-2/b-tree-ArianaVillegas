#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

template <typename T>
class BSTree; 

template <typename T>
class Node {
    unsigned int size;
    vector<T> keys;
    vector<Node<T>*> childs;
    bool isLeaf;

    public: 
        Node(unsigned int size, bool isLeaf = true) : size(size), isLeaf(isLeaf) {}

    template<class>
    friend class BTree; 
};

#endif