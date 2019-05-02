//
// Created by Yair Shachar on 12/01/2019.
//

#ifndef STATICEYE2_HASHTABLEALPHA_H
#define STATICEYE2_HASHTABLEALPHA_H
#include "DictListFinale.h"
#include "ImageAlpha.h"
#include "math.h"

#define MAX_LOAD_FACTOR 0.6
#define MIN_LOAD_FACTOR 0.25
#define DEFAULT_TABLE_SIZE 5

class ImageHashTable {
    int max_pixels;

    LinkedList<ImageAlpha*,int>* dynamic_array;
    int num_of_elements;
    int capacity;

public:
    ImageHashTable(int max_pixels):max_pixels(max_pixels){
        dynamic_array = new LinkedList<ImageAlpha*,int>[DEFAULT_TABLE_SIZE];
        num_of_elements=0;
        capacity=DEFAULT_TABLE_SIZE;
    }

    ~ImageHashTable(){
             ListNode<ImageAlpha*, int>* iterator;
        for (int i=0 ; i< capacity ; i++){
            iterator=dynamic_array[i].getFirst();
            while(iterator){
            delete iterator->data;
            iterator=iterator->next;
            }
        }
        delete[] dynamic_array;
    }

    int Hash(int image_ID){
        return image_ID%capacity;
    }

    bool AddImage(int image_id){
        int i = Hash(image_id);
        double alpha;
                ImageAlpha* new_image = new ImageAlpha(image_id,max_pixels);
        if(dynamic_array[i].AddElBack(new_image,image_id) != nullptr){

            num_of_elements++;
            alpha=(double)num_of_elements/capacity;
            if(alpha>=MAX_LOAD_FACTOR){
                EnlargeArraySize();
                return true;
            }
            return true;
        }
        delete new_image;
        return false;
    }

    bool DeleteImage(int image_id){
        int i = Hash(image_id);
        double alpha;
        if(dynamic_array== nullptr){
            return false;
        }
        LinkedList<ImageAlpha*,int>* list_ptr = &dynamic_array[i];
        ListNode<ImageAlpha*, int>* node_ptr = list_ptr->Find(image_id);
        if(node_ptr== nullptr){
            return false;
        }
        ImageAlpha* image_ptr =node_ptr->data;

        if(dynamic_array[i].PopElSpecific(image_id)){
            num_of_elements--;
            if(image_ptr != nullptr){
                delete image_ptr;
            }
            alpha=(double)num_of_elements/capacity;
            if(alpha<=MIN_LOAD_FACTOR){
                ShrinkArraySize();
                return true;
            }return true;
        }
        return false;
    }

    void EnlargeArraySize(){
        int new_capacity = 2*capacity+1;                                                              // new capacity will be twice as large as the old.
        LinkedList<ImageAlpha*,int>* new_array = new LinkedList<ImageAlpha*,int>[new_capacity];     // create a new array, size of new capacity.
        ReHash(new_array, new_capacity);                                                            // re-hash all elements into new array
    }

    void ShrinkArraySize(){
        if(capacity==DEFAULT_TABLE_SIZE){   // if table is at minimum table size, do nothing.
            return;
        }
        int new_capacity = ((capacity/2)<=DEFAULT_TABLE_SIZE)?DEFAULT_TABLE_SIZE:capacity/2; //if after shrink capacity will be under minimum, make capacity the minimum instead.
        LinkedList<ImageAlpha*,int>* new_array = new LinkedList<ImageAlpha*,int>[new_capacity];
        ReHash(new_array, new_capacity);
    }

    void ReHash(LinkedList<ImageAlpha*,int>* new_array, int new_capacity){
        int old_capacity = capacity;
        capacity=new_capacity;
        ListNode<ImageAlpha*, int>* iterator;

        int new_i;
        ListNode<ImageAlpha*, int>* iterator2= nullptr;
        for (int i = 0 ; i < old_capacity ; i++){// for each i in old array, run on i's linked list and add to the new array.
            iterator=dynamic_array[i].getFirst();

                while(iterator){

                new_i = Hash(iterator->key);
                new_array[new_i].AddElBack(iterator->data,iterator->key);
                iterator2=dynamic_array[i].getNext(iterator);
                dynamic_array[i].PopElSpecific(iterator->key);
                iterator=iterator2;
            }
        }// at this point, all elements of the old hash table have been copied to the new hashtable
        delete[] dynamic_array;     //freeing old array
        dynamic_array = new_array; // assigning new array
    }

    ImageAlpha* GetImage(int image_id){
        int i = Hash(image_id);
        ListNode<ImageAlpha*, int>* node_ptr = dynamic_array[i].Find(image_id);
        if(node_ptr== nullptr){
            return nullptr;
        }
        return node_ptr->data;
    }

    bool SetLabelScore(int image_id,int pixel, int label, int score){
         ImageAlpha* image_ptr = GetImage(image_id);
        if(image_ptr== nullptr){
            return false;
        }
        return image_ptr->SetLabelScore(pixel,label,score);
    }

    bool ResetLabelScore(int image_id, int pixel, int label){
        ImageAlpha* image_ptr = GetImage(image_id);
        if(image_ptr== nullptr){
            return false;
        }
        return image_ptr->ResetLabelScore(pixel,label);
    }
    int GetHighestScoredLabel(int image_id, int pixel, int label){
        ImageAlpha* image_ptr = GetImage(image_id);
        if(image_ptr== nullptr){
            return -1;
        }
        return image_ptr->GetHighestScoredLabel(pixel);
    }

    bool MergeSuperPixels(int image_id, int pixel1, int pixel2){
        ImageAlpha* image_ptr = GetImage(image_id);
        if(image_ptr== nullptr){
            return false;
        }
        return image_ptr->MergeSuperPixels(pixel1,pixel2);
    }

    int GetMaxPixels(){
        return max_pixels;
    }



    void PrintHashTable(){
        std::cout<<"PRINTING HASH TABLE:"<<std::endl;
        std::cout<<"Number of Elements: "<<num_of_elements<<std::endl;
        std::cout<<"Capacity: "<<capacity<<std::endl;
        std::cout<<"Printing Images: "<<std::endl;
        ListNode<ImageAlpha*, int>* iterator;

        for (int i = 0 ; i < capacity ; i++){
            iterator=dynamic_array[i].getFirst();
            SuperPixelNode* sp_ptr;
            while(iterator){
                std::cout<<"*********Image id: "<<iterator->key<<"*********"<<std::endl;
                for(int j=0; j<max_pixels ; j++){
                    sp_ptr=iterator->data->GetPixel(j);
                    sp_ptr->PrintPixel();

                }

                iterator=dynamic_array[i].getNext(iterator);
            }
        }
    }
};

#endif //STATICEYE2_HASHTABLEALPHA_H
