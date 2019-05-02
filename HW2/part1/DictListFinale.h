//
// Created by shira on 02/12/2018.
//

#ifndef CODEFILE_DICTLIST_H
#define CODEFILE_DICTLIST_H

#include <iostream>
#include <utility>
#include <stdexcept>
#include <cassert>
#include "DictListFinale.h"
//LABEL LIST

using std::ostream;
using std::istream;
using std::endl;
using std::cout;


template <class D, class K>
struct ListNode {
    D data;
    K key;
    ListNode* next;
    ListNode* previous;
    bool used_label;
    void makeUsed(){
        used_label=true;
    }
    void makeUnused(){
        used_label=false;
    }
    ListNode():next(nullptr),previous(nullptr),used_label(false){}
    ListNode(const D& data_input,const K& key_input, ListNode<D,K>* const next_input= nullptr,ListNode<D,K>* const previous_input= nullptr):data(data_input)
            ,key(key_input){
        next=next_input;
        previous=previous_input;
        used_label=false;
    }
    ~ListNode(){}


   /* ListNode(const ListNode<D,K>& node_input):data(node_input.data),key(node_input.key),next(new ListNode<D,K>),previous(new ListNode<D,K>){
        next=node_input.next;
        previous=node_input.previous;
    }*/
    /*ListNode& operator=(const Node& node_input){
        if(this == node_input){
            return *this;
        }
        next(node_input->next);
        previous(node_input->previous);
        data(node_input.data);
        key(node_input.key);
        return *this;
    }*/
    K& GetKey(){
        return key;
    }
};

template <class D,class K>
class LinkedList {
    ListNode<D, K> *first;
    ListNode<D, K> *last;
    int size;
public:
    LinkedList() : first(), last(), size(0) {
        first = nullptr;
        last = nullptr;
    }

    LinkedList(const ListNode<D, K> &first_input, const ListNode<D, K> &last_input, int size_input)
            : first(first_input), last(last_input), size(size_input) {}

    ~LinkedList() {
        ListNode<D, K> *iter1 = first;
        if(iter1){
         while(iter1->next!= nullptr){
             iter1=iter1->next;
             delete(iter1->previous);
             size--;
         }
         if(iter1){
             delete(iter1);
             size--;
         }
        }
    }
    LinkedList(const LinkedList<D, K> &list) : first(list.first), last(list.last), size(list.size) {

        ListNode<D, K> *temp = list.first;
        while (temp->next != nullptr) {
            ListNode<D, K> *new_element = new ListNode<D, K>(*temp);
            this->AddElBack(new_element->data, new_element->key);
            temp = temp->next;
        }
    }

    /*ConnectedList &operator=(const ConnectedList<D, K> &list) {
        if (this == list) {
            return *this;
        }
        first(list.first);
        last(list.last);
        size(list.size);
        return *this;
    }*/
    ListNode<D, K> *getFirst(){
        return first;
    }

    ListNode<D, K> *getNext(ListNode<D, K>* iter){
        if(iter== nullptr){
            return nullptr;
        } else return iter->next;
    }

    ListNode<D, K> *AddElBack(const D &data_input, const K &key_input) {
        ListNode<D, K> *new_first = new ListNode<D, K>(data_input, key_input);
        if (size == 0) {
            first = new_first;
            last = first;
            last->next = nullptr;
            last->previous= nullptr;
            size++;
            return new_first;
        } else if (size == 1) {
            first->next = new_first;
            new_first->previous=first;
            last = new_first;
            last->next = nullptr;
            size++;
            return new_first;
        } else {
            last->next = new_first;
            new_first->previous=last;
            last = new_first;
            last->next = nullptr;
            size++;
            return new_first;
        }
    }


    bool PopElSpecific(const K &key_input) {
        if (size == 0) {
            return false;
        } else if (size == 1) {
            if (first->key == key_input) {
                last = nullptr;
                delete first;
                first = nullptr;
                size--;
                return true;
            }
            return false;
        } else {
            ListNode<D, K> *temp = first->next;
            ListNode<D, K> *temp2 = first;
            if (first->key == key_input) {
                delete temp2;
                first = temp;
                size--;
                return true;
            }
            for (int i = 0; i < size - 1; i++) {
                if (temp->key == key_input && temp != last) {
                    temp2->next = temp->next;
                    delete temp;
                    size--;
                    return true;
                } else if (temp == last) {
                    delete temp;
                    temp2->next = nullptr;
                    last = temp2;
                    size--;
                    return true;
                }
                temp = temp->next;
                temp2 = temp2->next;
            }
        }
        return false;
    }

    ListNode<D, K> *Find(const K &key_input) const {
        ListNode<D, K> *temp = first;
        for (int i = 0; i < size; i++) {
            if (temp->key == key_input) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    const int Size() const {
        return size;
    }
    bool DeleteByPointer(const ListNode<D,K>* element){
        if (size == 0) {
            return false;
        } else if (size == 1) {
            if (first == element) {
                last = nullptr;
                delete first;
                first = nullptr;
                size--;
                return true;
            }
            return false;
        } else {
            ListNode<D, K> *pre = element->previous;
            if(pre== nullptr){
                first=element->next;
                first->previous= nullptr;
            } else{
            pre->next = element->next;
            }
            if(element->next== nullptr){
                last=element->previous;
            }else {
                element->next->previous = pre;
            }
            delete element;
            size--;
            return true;
        }
}
};

template <class D,class K>
class mapList{
    LinkedList<D,K> dictionary;
public:
    mapList():dictionary(){}
    ~mapList()= default;
    ListNode<D,K>* Add(const D& data_input,const K& key_input){
        return dictionary.AddElBack(data_input,key_input);
    }
    ListNode<D,K>* Find(const K& key_input){
        return dictionary.Find(key_input);
    }
    bool Delete(const K& key_input) {
        return dictionary.PopElSpecific(key_input);
    }
    int Size(){
        return dictionary.Size();
    }
    bool DeleteByPointer(const ListNode<D,K>* element){
        return dictionary.DeleteByPointer(element);
    }

};


#endif //CODEFILE_DICTLIST_H
