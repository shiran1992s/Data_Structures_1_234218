//
// Created by Yair Shachar on 09/12/2018.
//
#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <iostream>
//#include "ImageTagger.h"


template <class T, class K>
class Node{
public:
    int height;
    int balance_factor;
    int size;
    K key;
    T value;

    Node* left_son;
    Node* right_son;
    //Node* father;
    ~Node()= default;
    Node(K key,T value):height(-1),balance_factor(0),size(1),key(key),value(value),left_son(nullptr),right_son(nullptr){}//todo: when calling coonstructor- how do i create a new T / K value?
    Node():height(-1),balance_factor(0),size(0),key(nullptr),value(nullptr),right_son(nullptr),left_son(nullptr){}//todo: when calling default constructor- can i assign nullptr to value and key?

};

template<class T, class K>
class AVLtree {
private:
    //InsertNode:
    //Receives root of subtree to insert to, creates and inserts new node of key and value to AVL tree
    Node<T,K>* InsertNode(Node<T,K>* node,K key,T value);
    //BalanceNode(node): receives a node and performs rotations if necessary to keep AVL balanced.
    //returns the new root of balanced sub-tree
    Node<T,K>* BalanceNode(Node<T,K>* node);
    //RotationLL
    //performs LL rotation on specific node- returns the successor node.
    Node<T,K>* RotationLL(Node<T,K>* node);
    //RotationLR
    //performs LR rotation on specific node- returns the successor node.
    Node<T,K>* RotationLR(Node<T,K>* node);
    //RotationRR
    //performs RR rotation on specific node- returns the successor node.
    Node<T,K>* RotationRR(Node<T,K>* node);
    //RotationRL
    //performs RL rotation on specific node- returns the successor node.
    Node<T,K>* RotationRL(Node<T,K>* node);
    //
    //
    Node<T,K>* FindSuccessorForRemove(Node<T,K>* node);
    //FindFatherOfSuccessorForRemove: returns the father of the successor, Assumes node has 2 sons.
    //
    Node<T,K>* FindFatherOfSuccessorForRemove(Node<T,K>* node);
    //
    //
    Node<T,K>* RemoveNode(Node<T,K>* node,K key);
    //IsLeaf
    //return true if node is a leaf false if not
    bool IsLeaf(Node<T,K>* node);
    //Is1SonNode
    //returns true if node has only 1 son, false if not
    bool Is1SonNode(Node<T,K>* node);
    //Is2SonNode
    //returns true if node has 2 sons, false if not
    bool Is2SonNode(Node<T,K>* node);
    //RemoveLeaf
    //Removes a *verified* leaf from the AVL tree (assumes node is leaf) // todo: should i add a safety check to check if is leaf? what happens when null pointer is received?<-- right now assuming i checked and this is not possible (must remove something that exists in the tree)
    Node<T,K>* RemoveLeaf(Node<T,K>* node);
    //
    //
    Node<T,K>* Remove1SonNode(Node<T,K>* node);
    //
    //
    Node<T,K>* Remove2SonNode(Node<T,K>* node);
    //
    //
    void UpdateHeightAndBalanceFactor(Node<T,K>* node);
    //
    //
    bool IsEmpty();
    //
    //
    void PostOrderDelete(Node<T,K>* node);

public:
    int size_of_tree;
    Node<T,K>* root;

    //
    //Constructor
    AVLtree();
    //
    //
    ~AVLtree(){
        PostOrderDelete(root);
    }

    // Add
    // Inserts a new node with key and value. to the AVL tree.
    // Returns a pointer to the inserted node.
    Node<T,K>* Add(K key, T value);

    // Find
    // Returns pointer to node with specific key
    Node<T,K>* Find(Node<T,K>* node, K key);

    //
    //
    T FindValue(K key){
        Node<T,K>* temp=Find(root,key);
        if(temp== nullptr){
            return nullptr;
        }
        return Find(root,key)->value;
    }


    // Delete
    // Removes node with key(key) from AVL tree and releases it.
    // Returns true if successful , false else
    bool Delete(K key);

    // DeleteByPointer(node): removes Node pointed by node from AVL tree.
    // Returns true if successful , false else
    bool DeleteByPointer(Node<T,K>* node);

    // Size
    // Returns the number of nodes in the AVL tree.
    int Size(){
        return size_of_tree;
    }

    // Quit
    // empties and releases AVL tree.
    void Quit(){
        delete(this);
    }

    // Insert(key,value)
    // creates and inserts a new node with key,value to the AVL tree.
    // returns: true if successful , false else
    bool Insert(K key, T value);

    // IsInTree(key)
    // Returns true if is in tree, false else.
    bool IsInTree(Node<T,K>* node, K key);

    //GetRootNode
    //Returns the Root of the AVL tree
    Node<T,K>* GetRootNode();
    void PrintInOrder(Node<T,K>* node);
    void PrintTree();
};

template<class T, class K>
Node<T, K> *AVLtree<T, K>::InsertNode(Node<T, K> *node, K key, T value) {
    if (node == nullptr) {
        node = new Node<T, K>(key, value);
        UpdateHeightAndBalanceFactor(node);
        return(BalanceNode(node));
    } else if (key < node->key) { // if key < node->key go left in binary tree, left_son will become inserted node if NULL or updated to successor node after balancing
        node->left_son = InsertNode(node->left_son, key, value);
    } else {                    // if key > node->key go left in binary tree, right_son will become inserted node if NULL or updated to successor node after balancing
        node->right_son = InsertNode(node->right_son, key, value);
    }
    UpdateHeightAndBalanceFactor(node);
    return(BalanceNode(node));
}

template<class T, class K>
void AVLtree<T, K>::UpdateHeightAndBalanceFactor(Node<T, K> *node) {
    int left_son_height = (node->left_son== nullptr)?-1:node->left_son->height;
    int right_son_height = (node->right_son== nullptr)?-1:node->right_son->height;
    int left_son_size = (node->left_son== nullptr)?0:node->left_son->size;
    int right_son_size = (node->right_son== nullptr)?0:node->right_son->size;
    node->height=(left_son_height>=right_son_height)?left_son_height+1:right_son_height+1;
    node->balance_factor=left_son_height-right_son_height;
    node->size=left_son_size+right_son_size+1;

}

template<class T, class K>
bool AVLtree<T, K>::IsEmpty() {
    return (size_of_tree==0&&root== nullptr)?true:false;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::BalanceNode(Node<T, K> *node) {
    if(node== nullptr){
        return node;
    }
    if(node->balance_factor==2){
        if(node->left_son->balance_factor>=0){
            return(RotationLL(node));
        } else if(node->left_son->balance_factor==-1) {
            return(RotationLR(node));
        }
    }
    if(node->balance_factor==-2){
        if (node->right_son->balance_factor <= 0) {
            return (RotationRR(node));
        } else {
            if (node->right_son->balance_factor == 1) {
                return RotationRL(node);
            }
        }
    }
    return node;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RotationLL(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }
    Node<T,K>* successor=node->left_son;
    node->left_son=successor->right_son;

    UpdateHeightAndBalanceFactor(node);
    successor->right_son=node;
    UpdateHeightAndBalanceFactor(successor);
    return successor;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RotationLR(Node<T, K> *node) {
    node->left_son=RotationRR(node->left_son);
    return(RotationLL(node));
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RotationRR(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }
    Node<T,K>* successor=node->right_son;
    node->right_son=successor->left_son;
    UpdateHeightAndBalanceFactor(node);
    successor->left_son=node;
    UpdateHeightAndBalanceFactor(successor);
    return successor;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RotationRL(Node<T, K> *node) {
    node->right_son=RotationLL(node->right_son);
    return(RotationRR(node));
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::FindSuccessorForRemove(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }
    if(Is1SonNode(node)){
        return (node->left_son== nullptr)?node->right_son:node->left_son;
    }
    if(Is2SonNode(node)){
        Node<T,K>* iterator = node->right_son;
        while(iterator->left_son){
            iterator=iterator->left_son;
        }
        return iterator;
    }
    return nullptr;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::FindFatherOfSuccessorForRemove(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }
    Node<T,K>* iterator = node->right_son;
    Node<T,K> *iterator2 = nullptr;
    while(iterator->left_son){
        iterator2=iterator;
        iterator=iterator->left_son;
    }
    return iterator2;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RemoveNode(Node<T, K> *node, K key) {
    if(key==node->key){
        if(IsLeaf(node)){
            return RemoveLeaf(node);
        }
        if(Is1SonNode(node)){
            return Remove1SonNode(node);
        }
        if(Is2SonNode(node)){
            return Remove2SonNode(node);
        }
    }
    else if (key<node->key){
        node->left_son=RemoveNode(node->left_son,key);
    } else if(key>node->key){
        node->right_son=RemoveNode(node->right_son,key);
    }
    UpdateHeightAndBalanceFactor(node);
    return(BalanceNode(node));
}

template<class T, class K>
bool AVLtree<T, K>::IsLeaf(Node<T, K> *node) {
    if (node== nullptr){
        return false;
    }
    return node->left_son == nullptr && node->right_son == nullptr;
}

template<class T, class K>
bool AVLtree<T, K>::Is1SonNode(Node<T, K> *node) {
    if(node== nullptr){
        return false;
    }
    if(node->left_son){
        return node->right_son== nullptr;
    }
    if(node->right_son){
        return node->left_son== nullptr;
    }
    return false;
}

template<class T, class K>
bool AVLtree<T, K>::Is2SonNode(Node<T, K> *node) {
    if(node== nullptr){
        return false;
    }
    return node->right_son&&node->left_son;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::RemoveLeaf(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }
    if(node==root){
        Node<T,K>* temp=root;
        delete(temp);
        root= nullptr;
        return root;
    }
    delete(node);
    return nullptr;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::Remove2SonNode(Node<T, K> *node) {
    if(node== nullptr){
        return nullptr;
    }

    Node<T,K>* temp= nullptr;
    Node<T,K>* successor = FindSuccessorForRemove(node);
    if(successor==node->right_son){
        successor->left_son=node->left_son;
        node->right_son=successor->right_son;
        node->left_son= nullptr;
        successor->right_son=RemoveNode(node,node->key);
        UpdateHeightAndBalanceFactor(successor);
        successor=BalanceNode(successor);
        return successor;
    }

    Node<T,K>* successor_father = FindFatherOfSuccessorForRemove(node);
    successor->left_son=node->left_son;
    successor_father->left_son=node;
    node->left_son= nullptr;
    temp=node->right_son;
    node->right_son=successor->right_son;
    successor->right_son=temp; // todo: do i need this temp?
    successor->right_son=RemoveNode(successor->right_son,node->key);
    UpdateHeightAndBalanceFactor(successor);
    successor=BalanceNode(successor);
    return successor;
}

template<class T, class K>
void AVLtree<T, K>::PrintInOrder(Node<T, K> *node) {
    if(node== nullptr){
        return;
    }
    if(node->left_son){
        PrintInOrder(node->left_son);
    }
    std::cout<<"key "<<node->key<<" height: "<<node->height<<" BF "<<node->balance_factor<<" size: "<<node->size<<std::endl;
    if(node->right_son){
        PrintInOrder(node->right_son);
    }
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::Remove1SonNode(Node<T, K> *node) {
    Node<T,K>* successor=(node->left_son)!= nullptr?node->left_son:node->right_son;
    if(node==root){
        Node<T,K>* temp = root;
        root=successor;
        delete(temp);
        return  successor;
    }
    delete(node);
    return successor;
}

template<class T, class K>
bool AVLtree<T, K>::Insert(K key, T value) { //todo: should i check if key/value are null?
    if(IsInTree(root,key)){ //checking if key is in tree (returning false if is)
        return false;
    }
    root=InsertNode(root,key,value);
    if(IsInTree(root,key)){
        size_of_tree++;
        return true;
    }   return false;
}

template<class T, class K>
bool AVLtree<T, K>::Delete(K key) {
    if(!IsInTree(root,key)){
        return false;
    } else
        root=RemoveNode(root,key);
    if(!IsInTree(root,key)) {
        size_of_tree--;
        return true;
    }
    return false;
}

template<class T, class K>
bool AVLtree<T, K>::IsInTree(Node<T, K> *node, K key) {
    if(node== nullptr){
        return false;
    }
    if(key<node->key){
        return IsInTree(node->left_son,key);
    }
    if(key>node->key){
        return IsInTree(node->right_son,key);
    }
    if(key==node->key){
        return true;
    }
    return false;
}

template<class T, class K>
void AVLtree<T, K>::PrintTree() {
    std::cout<<"printing tree..."<<std::endl;
    std::cout<<"tree size: "<<size_of_tree<<std::endl;
    PrintInOrder(root);
}

template<class T, class K>
AVLtree<T, K>::AVLtree():size_of_tree(0),root(nullptr){}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::Find(Node<T, K> *node, K key) {
    if(node== nullptr){
        return nullptr;
    }
    if(node->key==key){
        return node;
    }
    else return Find(key < node->key ? node->left_son : node->right_son, key);

}



template<class T, class K>
void AVLtree<T, K>::PostOrderDelete(Node<T, K> *node) {
    if(node== nullptr){
        return;
    }
    if(node->left_son){
        PostOrderDelete(node->left_son);
    }
    if(node->right_son){
        PostOrderDelete(node->right_son);
    }
    node->left_son= nullptr;
    node->right_son= nullptr;
    delete(node);
    node= nullptr;
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::Add(K key, T value) {
    if(!Insert(key,value)){
        return nullptr;
    }
    else return Find(root, key);
}

template<class T, class K>
bool AVLtree<T, K>::DeleteByPointer(Node<T, K> *node) {
    return Delete(node->key);
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::GetRootNode() {
    return root;
}



#endif //AVLTREE_AVLTREE_H
