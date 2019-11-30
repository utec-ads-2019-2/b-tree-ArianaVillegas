#ifndef BTREE_H
#define BTREE_H

#include <algorithm>
#include <vector>
#include "node.h"

using namespace std;

template <typename T>
class BTree {
    private:
        typedef typename vector<T>::iterator keyIte;
        typedef typename vector<Node<T>*>::iterator childIte;

        Node<T>* root;
        unsigned int degree;

        bool find(T data, Node<T>* &node, int &i){
            while(node){
                for(i=0; i<node->keys.size(); ++i) {
                    if(data==node->keys[i]) return true;
                    else if(data<node->keys[i]) break;
                }
                if(!node->isLeaf) node = node->childs[i];
                else node = 0;
            }
            return false;
        }

        void insKeys(Node<T>* &node1, Node<T>* &node2, int pos){
            node1->keys.insert(node1->keys.begin(),node2->keys.begin()+pos+1,node2->keys.end());
            node2->keys.erase(node2->keys.begin()+pos,node2->keys.end());
        }

        void insChilds(Node<T>* &node1, Node<T>* &node2, int pos){
            node1->childs.insert(node1->childs.begin(),node2->childs.begin()+pos+1,node2->childs.end());
            node2->childs.erase(node2->childs.begin()+pos+1,node2->childs.end());
        }

        void split(T &data, Node<T>* &node){
            auto pos = degree/2;
            data = node->keys[pos];
            Node<T>* newNode = new Node<T>(degree,node->isLeaf); 
            insKeys(newNode,node,pos);
            if(!node->isLeaf) {
                insChilds(newNode,node,pos);
            }
            node = newNode;
        }        

        bool insert(T &data, Node<T>* &node){
            int i;
            T actual;
            for(i=0; i<node->keys.size(); ++i)
                if(data<=node->keys[i]) break;
            if(!node->isLeaf){
                auto temp = node->childs[i];
                if(insert(data,temp)){
                    node->keys.insert(node->keys.begin()+i,data);
                    node->childs.insert(node->childs.begin()+i+1,temp);
                }
            } else node->keys.insert(node->keys.begin()+i,data);
            if(node->keys.size()==degree){
                split(data,node);
                return true;
            }
            return false;
        }

        void findMaxLeft(Node<T>* &node){
            while(!node->isLeaf)
                node=node->childs.back();
        }

        void rotate(Node<T>* &node, Node<T>* n1, Node<T>* n2, int n3, int pos1, int pos2, int pos3, int pos4){
            n1->keys.insert(n1->keys.begin()+pos2,node->keys[n3]);
            node->keys[n3] = n2->keys[pos1];
            n2->keys.erase(n2->keys.begin()+pos1);
            if(!n1->isLeaf) {
                n1->childs.insert(n1->childs.begin()+pos2+pos3,n2->childs[pos1+pos4]);
                n2->childs.erase(n2->childs.begin()+pos1+pos4);
            }
        }

        void merge(Node<T>* node, Node<T>* n1, Node<T>* n2, int n3, int pos1, int pos2){
            n1->keys.insert(n1->keys.begin()+pos1,node->keys[n3]); 
            node->keys.erase(node->keys.begin()+n3);
            n1->keys.insert(n1->keys.begin()+pos1,n2->keys.begin(),n2->keys.end());
            if(!n2->isLeaf) {
                n1->childs.insert(n1->childs.begin()+pos1+pos2,n2->childs.begin(),n2->childs.end());
            }
        }

        bool remove(T data, T* &temp, Node<T>* &node){
            int i;
            for(i=0; i<node->keys.size(); ++i){
                if(data<=node->keys[i]) break;
            }
            if(node->isLeaf){
                if(data != node->keys[i] && !temp) return false;
                if(i==node->keys.size()) --i;
                if(temp && *temp != node->keys[i]) swap(*temp,node->keys[i]);
                node->keys.erase(node->keys.begin()+i);
                return true;
            }
            if(i<node->keys.size() && data == node->keys[i]) temp=&node->keys[i];
            if(!remove(data,temp,node->childs[i])) return false;
            auto size=node->childs[i]->keys.size();
            if(size<(degree/2)){
                if(i && node->childs[i-1]->keys.size()>(degree/2)){
                    rotate(node,node->childs[i],node->childs[i-1],i-1,node->childs[i-1]->keys.size()-1,0,0,1);
                } else if((i+1)<node->childs.size() && node->childs[i+1]->keys.size()>(degree/2)){
                    rotate(node,node->childs[i],node->childs[i+1],i,0,size,1,0);
                } else {
                    if(i) merge(node,node->childs[i-1],node->childs[i],i-1,node->childs[i-1]->keys.size(),1);
                    else merge(node,node->childs[i+1],node->childs[i],i,0,0);
                    delete node->childs[i]; node->childs.erase(node->childs.begin()+i);
                    if(node == root && root->keys.empty()) {root=root->childs.front();delete node;}
                }
            }
            return true;
        }

        void traverseInOrder(Node<T>* node) {
            int i;
            for(i=0; i<node->keys.size(); ++i){
                if(!node->isLeaf) traverseInOrder(node->childs[i]);
                cout << node->keys[i] << ' ';
            }
            if(!node->isLeaf) traverseInOrder(node->childs[i]);
        }

        void deleteAll(Node<T>* node){
            int i;
            for(i=0; i<node->keys.size(); ++i){
                if(!node->isLeaf) deleteAll(node->childs[i]);
            }
            if(!node->isLeaf) deleteAll(node->childs[i]);
            delete node;
        }

    public:
        BTree(unsigned int degree) : degree(degree), root(nullptr) {
            root = new Node<T>(degree);
        }

        bool search(T k) { 
            auto temp = root; int i;
            find(k,temp,i);
        } 

        bool insert(T k) {
            auto temp = root;
            if(insert(k,temp)){
                Node<T>* newNode = new Node<T>(degree,false);
                newNode->keys.push_back(k);
                newNode->childs.push_back(root);
                newNode->childs.push_back(temp);
                temp = newNode;
            }
            root = temp;
            return true;
        }

        bool remove(T k) {
            T *temp=0;
            Node<T> *node=root;
            return remove(k,temp,node);
        }

        void print() {
            traverseInOrder(root);
            cout << endl;
        }

        ~BTree(){
            deleteAll(root);
        }
};

#endif