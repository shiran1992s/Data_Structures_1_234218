//
// Created by Yair Shachar on 09/01/2019.
//

#ifndef STATICEYE_SCORERANKEDAVLTREE_H
#define STATICEYE_SCORERANKEDAVLTREE_H
#include <stdio.h>
#include <iostream>
#include <math.h>
using std::cout;

template <class T, class K>
class Node{
public:
    int height;
    int balance_factor;
    int size;

    int score;
    int max_score;
    int max_scored_label;

    K key;
    T value;

    Node* left_son;
    Node* right_son;
    //Node* father;
    ~Node()= default;
    /*{
        //delete(this->value);
    }*/
    Node(K key,T value,int score):height(-1),balance_factor(0),size(1),score(score),max_score(score),max_scored_label((int)key),key(key),value(value),left_son(nullptr),right_son(nullptr){}
    Node():height(-1),balance_factor(0),size(0),score(0),max_score(0),key(0),value(0),left_son(nullptr),right_son(nullptr){}
    Node(const Node& node_inp):height(node_inp.height),balance_factor(node_inp.balance_factor),size(node_inp.size) // copy constructor
    ,score(node_inp.score),max_score(node_inp.max_score),max_scored_label(node_inp.max_scored_label),key(node_inp.key)
    ,value(node_inp.value),left_son(nullptr),right_son(nullptr){}
    Node& operator=(const Node& node_inp){// operator= , only the score , max_score , and max_scored_label
         score=node_inp.score;
         max_score=node_inp.max_score;
         max_scored_label=node_inp.max_scored_label;
         key = node_inp.key;
         value = node_inp.value;
        return *this;
    }
    bool operator==(const Node<T,K>& node_inp1){
        return (key == node_inp1.key);
    }// operator== , checks if the nodes have the same keys , Simetric operator - implementation outside of the class
    bool operator<(const Node& node_inp){// operator< , checks if the node have lower key
        return (key < node_inp.key);
    }
    bool operator>(const Node& node_inp){// operator> , checks if the node have bigger key
        return (!(key < node_inp.key) && (key != node_inp.key));
    }
    Node& operator+=(const Node& node_inp){// operator+= , combines the scores and the values of 2 nodes
        score+=node_inp.score;
        value+=node_inp.value;
        return *this;
    }
};

template<class T, class K>
class AVLtree {
private:
    //InsertNode:
    //Receives root of subtree to insert to, creates and inserts new node of key and value to AVL tree
    Node<T,K>* InsertNode(Node<T,K>* node,K key,T value, int score);
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
    //Removes a *verified* leaf from the AVL tree (assumes node is leaf)
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




public:
    int size_of_tree;
    Node<T,K>* root;

    //
    //Constructor
    AVLtree():size_of_tree(0),root(nullptr){}
    //
    //
    ~AVLtree(){
        PostOrderDelete(root);
    }

    // Add
    // Inserts a new node with key and value. to the AVL tree.
    // Returns a pointer to the inserted node.
    Node<T,K>* Add(K key, T value, int score);

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
    bool Insert(K key, T value,int score);

    // IsInTree(key)
    // Returns true if is in tree, false else.
    bool IsInTree(Node<T,K>* node, K key);

    //GetRootNode
    //Returns the Root of the AVL tree
    Node<T,K>* GetRootNode();

//
//Trims an empty Complete tree to have n nodes

    Node<T,K>* TrimEmptyCompleteTree(Node<T,K>* node, int n){
        if(node== nullptr){
            return nullptr;
        }
        if(node->right_son){
            node->right_son=(TrimEmptyCompleteTree(node->right_son, n));
        }
        if(n<this->size_of_tree && node->right_son== nullptr && node->left_son== nullptr){
            delete(node);
            size_of_tree--;
            return nullptr;
        }
        UpdateHeightAndBalanceFactor(node);
        if(node->left_son){
            node->left_son=(TrimEmptyCompleteTree(node->left_son, n));
        }
        UpdateHeightAndBalanceFactor(node);
        return node;
    }

    //post order update BF and Ranks
    Node<T,K>* PostOrderUpdateTree(Node<T,K>* node){
        if(node== nullptr){
            return nullptr;
        }
        if(node->left_son){
            node->left_son=PostOrderUpdateTree(node->left_son);
        }
        if(node->right_son){
            node->right_son=PostOrderUpdateTree(node->right_son);
        }
        UpdateHeightAndBalanceFactor(node);
        return node;
    }

//MakeEmptyCompleteTree: returns a root of an empty complete AVLtree with height "height"

    Node<T,K>* MakeEmptyCompleteTree(int height){
        if(height<0){
            return nullptr;
        }
        Node<T,K>* node = new Node<T,K>();
        node->left_son=MakeEmptyCompleteTree(height-1);
        node->right_son=MakeEmptyCompleteTree(height-1);
        UpdateHeightAndBalanceFactor(node);
        return node;
    }

    void PrintInOrder(Node<T,K>* node);
    void PrintTree();
    void PostOrderDelete(Node<T,K>* node);
    void FindAndUpdateScore(Node<T, K> *node, K key, int score);

    };



template <class T, class K>
AVLtree<T,K>* CreateEmptyCompleteTree(int height){
    if(height<0){
        return nullptr;
    }
    AVLtree<T,K>* emptyTree = new AVLtree<T,K>();
    emptyTree->root=emptyTree->MakeEmptyCompleteTree(height);
    emptyTree->size_of_tree=emptyTree->root->size;
    return emptyTree;
}


template<class T, class K>
void InOrderFillArray(Node<T,K>* node, Node<T,K>** node_array, int* i ){// fills an empty array according to InOrder search , starts always from i=0 and i++ between left and right sons.
    if(node== nullptr){
        return;
    }
    if(node->left_son){
        InOrderFillArray(node->left_son,node_array,i);

    }
    node_array[*i] = new Node<T,K>(*node);
    (*i)++;
    if(node->right_son){
        InOrderFillArray(node->right_son,node_array,i);
    }
}
template<class T, class K>
void InOrderFillEmptyTree(Node<T,K>* node, Node<T,K>** node_array, int* i,int size ){// fills an empty array according to InOrder search , starts always from i=0 and i++ between left and right sons.
    if(node== nullptr || (*i)>=size){
        return;
    }
    if(node->left_son){
        InOrderFillEmptyTree(node->left_son,node_array,i,size);

    }
    if((*i)>=size){
        return;
    }
    *node = *node_array[*i];
    (*i)++;
    if((*i)>=size){
        return;
    }
    if(node->right_son){
        InOrderFillEmptyTree(node->right_son,node_array,i,size);
    }
}
template<class T, class K>
void FillEmptyTreeFromArray(AVLtree<T,K>* tree,Node<T,K>** node_array,int size){// Fills empty tree from array
    int index=0;
    InOrderFillEmptyTree<T,K>(tree->root,node_array,&index,size);
    return;
}

template<class T, class K>
Node<T,K>** CreateTreeArray(AVLtree<T,K>* tree){// Creates Node** array
    if(tree== nullptr){
        return nullptr;
    }
    Node<T,K>** node_array = new Node<T,K>*[tree->size_of_tree];
    int index=0;
    InOrderFillArray<T,K>(tree->root,node_array,&index);
    return node_array;
}
template<class T, class K>
int TotalSizeArray(Node<T,K>** node_array1,int size1,  Node<T,K>** node_array2,int size2){// returns the total necessary size of the array , takes to consideration Nodes with same key
    int counter=0;
    int i=0,j=0;
    for(; i<size1 && j<size2;) {
        if (*node_array1[i] < *node_array2[j]) {
            counter++;
            i++;
        } else if (*node_array1[i] > *node_array2[j]) {
            counter++;
            j++;
        } else {
            counter++;
            i++;
            j++;
        }
    }
    for(; i<size1 ; i++){
        counter++;
    }
    for(; j<size2 ; j++){
        counter++;
    }
    return counter;
}



template<class T, class K>
Node<T,K>** MergeTreeArrays(AVLtree<T,K>* tree1,AVLtree<T,K>* tree2, int* total_size){// makes MergeSort between two tree arrays into 1 total one , takes to consideration which Node has smaller key
    int size1 = tree1== nullptr?0:tree1->size_of_tree;
    int size2 = tree2== nullptr?0:tree2->size_of_tree;
    Node<T,K>** node_array1 = CreateTreeArray(tree1);
    Node<T,K>** node_array2 = CreateTreeArray(tree2);
    *total_size = TotalSizeArray(node_array1,size1,node_array2,size2);
    Node<T,K>** total_node_array = new Node<T,K>*[*total_size];
    int i=0,j=0,k=0;
    for(; i<size1 && j<size2;k++) {
        if (*node_array1[i] < *node_array2[j]) {
            total_node_array[k] = new Node<T,K>(*node_array1[i]);
            i++;
        } else if (*node_array1[i] > *node_array2[j]) {
            total_node_array[k] = new Node<T,K>(*node_array2[j]);
            j++;
        } else {
            Node<T,K>* united = new Node<T,K>(*node_array1[i]);
            *united+=*node_array2[j];
            total_node_array[k] = united;
            i++;
            j++;
        }
    }
    for(; i<size1 ; i++,k++){
        total_node_array[k] = new Node<T,K>(*node_array1[i]);
    }
    for(; j<size2 ; j++,k++){
        total_node_array[k] = new Node<T,K>(*node_array2[j]);
    }
    DeleteArray(node_array1,size1);
    DeleteArray(node_array2,size2);
    return total_node_array;
}
template<class T, class K>
void DeleteArray(Node<T,K>** node_array1,int size){ // Delete an allocated Node** array, to prevent memory leek
    for(int i=0;i<size;i++){
        delete node_array1[i];
    }
    delete[] node_array1;
}

int FindMinCompleteTreeHeight(int num_of_nodes){
    return ceil((double)log2(num_of_nodes+1))-1;
}

template <class T, class K>
AVLtree<T,K>* MergeTrees(AVLtree<T,K>* t1, AVLtree<T,K>* t2){
    if(t1 == nullptr || t2 == nullptr){
        return nullptr;
    }
    if((t1->root== nullptr && t2->root== nullptr)){
        return nullptr;
    }
    int total_size;
    int height;
    Node<int,int>** node_array = MergeTreeArrays(t1,t2,&total_size);
    height = FindMinCompleteTreeHeight(total_size);
    AVLtree<int,int>* merged_tree=CreateEmptyCompleteTree<int,int>(height);
    merged_tree->TrimEmptyCompleteTree(merged_tree->root,total_size);
    FillEmptyTreeFromArray<T,K>(merged_tree,node_array,total_size);
    merged_tree->PostOrderUpdateTree(merged_tree->root);
    DeleteArray(node_array,total_size);
    return merged_tree;

}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::InsertNode(Node<T, K> *node, K key, T value, int score) {
    if (node == nullptr) {
        node = new Node<T, K>(key, value,score);
        UpdateHeightAndBalanceFactor(node);
        Node<T, K>* balanced_node = BalanceNode(node);
        UpdateHeightAndBalanceFactor(balanced_node);
        return(balanced_node);
    } else if (key < node->key) { // if key < node->key go left in binary tree, left_son will become inserted node if NULL or updated to successor node after balancing
        node->left_son = InsertNode(node->left_son, key, value,score);
    } else {                    // if key > node->key go left in binary tree, right_son will become inserted node if NULL or updated to successor node after balancing
        node->right_son = InsertNode(node->right_son, key, value,score);
    }
    UpdateHeightAndBalanceFactor(node);
    Node<T, K>* balanced_node = BalanceNode(node);
    UpdateHeightAndBalanceFactor(balanced_node);
    return(balanced_node);
}

template<class T, class K>
void AVLtree<T, K>::UpdateHeightAndBalanceFactor(Node<T, K> *node) {
    int left_son_height = (node->left_son== nullptr)?-1:node->left_son->height;
    int right_son_height = (node->right_son== nullptr)?-1:node->right_son->height;
    int left_son_size = (node->left_son== nullptr)?0:node->left_son->size;
    int right_son_size = (node->right_son== nullptr)?0:node->right_son->size;

    int left_son_max_score=(node->left_son== nullptr)?0:node->left_son->max_score;
    int right_son_max_score = (node->right_son== nullptr)?0:node->right_son->max_score;

    int left_son_max_scored_label = (node->left_son== nullptr)?0:node->left_son->max_scored_label;
    int right_son_max_scored_label = (node->right_son== nullptr)?0:node->right_son->max_scored_label;

    node->height=(left_son_height>=right_son_height)?left_son_height+1:right_son_height+1;
    node->balance_factor=left_son_height-right_son_height;
    node->size=left_son_size+right_son_size+1;

    FindMaxLabel(node,left_son_max_score,right_son_max_score,left_son_max_scored_label,right_son_max_scored_label);
}

template <class T, class K>
void FindMaxLabel(Node<T, K> *node,int msl,int msr,int msll,int mslr){
    if(node == nullptr){
        return;
    }
    if(node->score<=msr) {
        if(msr >= msl){
            node->max_scored_label = mslr;
            node->max_score = msr;

        }else{
            node->max_scored_label = msll;
            node->max_score = msl;
        }
    }else if(node->score < msl){
        node->max_scored_label = msll;
        node->max_score = msl;
    }else{
        node->max_scored_label = (int)node->key;
        node->max_score = node->score;
    }
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
    UpdateHeightAndBalanceFactor(node);
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
    UpdateHeightAndBalanceFactor(node);
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
    Node<T, K>* balanced_node = BalanceNode(node);
    UpdateHeightAndBalanceFactor(balanced_node);
    return(balanced_node);
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
    successor->right_son=temp;
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
    std::cout<<"label "<<node->key<<" score: "<<node->score<<" height: "<<node->height<<" BF "<<node->balance_factor<<" size: "<<node->size<<" wich is label: "<<node->max_scored_label<<std::endl;
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
    UpdateHeightAndBalanceFactor(successor);
    return successor;
}

template<class T, class K>
bool AVLtree<T, K>::Insert(K key, T value, int score) {
    if(IsInTree(root,key)){ //checking if key is in tree (returning false if is)
        return false;
    }
    root=InsertNode(root,key,value,score);
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
    if(root== nullptr){
        std::cout<<"Label Tree is empty"<<std::endl;
        return;
    }
    std::cout<<"max score is: "<<root->max_score<<std::endl;
    std::cout<<"max scored label is: "<<root->max_scored_label<<std::endl;

    PrintInOrder(root);
}

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
void  AVLtree<T, K>::FindAndUpdateScore(Node<T, K> *node, K key, int score) {
    if(node == nullptr){
        return;
    }
    if(node->key < key){
        FindAndUpdateScore(node->right_son,key,score);
        UpdateHeightAndBalanceFactor(node);
    }else if(node->key > key){
        FindAndUpdateScore(node->left_son,key,score);
        UpdateHeightAndBalanceFactor(node);
    }else{
        node->score=score;
        UpdateHeightAndBalanceFactor(node);
    }
    UpdateHeightAndBalanceFactor(node);
    return   ;
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
Node<T, K> *AVLtree<T, K>::Add(K key, T value, int score) {
    Node<T,K>* node_ptr = Find(root, key);
    if(node_ptr){
        FindAndUpdateScore(root,key,score);
        return node_ptr;
    } else
    if(!Insert(key,value,score)){
        return nullptr;
    }
    return Find(root, key);
}

template<class T, class K>
bool AVLtree<T, K>::DeleteByPointer(Node<T, K> *node) {
    return Delete(node->key);
}

template<class T, class K>
Node<T, K> *AVLtree<T, K>::GetRootNode() {
    return root;
}



#endif //STATICEYE_SCORERANKEDAVLTREE_H
