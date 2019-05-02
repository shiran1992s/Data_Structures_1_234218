//
// Created by Yair Shachar on 12/12/2018.
//

#include "AVLtree.h"
#include "DictListFinale.h"
#include <iostream>

#ifndef IMAGETAGGER_IMAGETAGGER_H
#define IMAGETAGGER_IMAGETAGGER_H



class Image{
public:
    int imageID;
    ListNode<int,int>** segment_ptr_array;
    LinkedList<int,int>* label_list;
    LinkedList<int,int>* unused_label_list;


    Image(int segments, int imageID){
        this->imageID=imageID;
        segment_ptr_array = new ListNode<int,int>*[segments];
        label_list= new LinkedList<int,int>;
        unused_label_list= new LinkedList<int,int>;
        //ListNode<int,int>** t=segment_ptr_array;
        //initialise pointers in segment ptr array to point to the relevant labels (all unused in initialization)
        for(int i = 0 ; i < segments ; i++){
            segment_ptr_array[i]=unused_label_list->AddElBack(i,i);
        }
    }


    ~Image() {
            if(label_list){
            delete(label_list);
        }
       if (unused_label_list) {
           delete (unused_label_list);
       }
       if(segment_ptr_array){
        delete[](segment_ptr_array);
        }
        label_list= nullptr;
       unused_label_list= nullptr;
       segment_ptr_array= nullptr;
       imageID=0;
    }
    ;

    //
    //Returns a pointer to Label pointed by segment_ptr_array[segmentID] (can be Used or Unused)
    ListNode<int,int>* GetSegment(int segmentID){
      return segment_ptr_array[segmentID];
    }


    bool isUsedSegment(int segmentID){
       // ListNode<int,int>* a = GetSegment(segmentID);
        return (GetSegment(segmentID)->used_label);
    }

};


class ImageTree: public AVLtree<Image*,int> {
public:
    int segments;
    ImageTree(int segments):segments(segments){};
    ~ImageTree(){
        PostOrderDeleteImage(root);
        root= nullptr;
      }

    void PostOrderDeleteImage(Node<Image*, int>* node){
        if(node== nullptr){
            return;
        }
        if(node->left_son){
            PostOrderDeleteImage(node->left_son);
        }
        if(node->right_son){
            PostOrderDeleteImage(node->right_son);
        }
        node->left_son= nullptr;
        node->right_son= nullptr;
        //delete(node->value);
        delete(node);
         }
    //GetTotalAmountOfLabel
    //Returns the total amount a specific label appears in the image tree
    int GetTotalAmountOfLabel(int label, Node<Image *, int> *node) {
        if (node == nullptr) {
            return 0;
        }
        int i = 0;
        if (node->left_son) {
            i += GetTotalAmountOfLabel(label, node->left_son);
        }

        ListNode<int, int> *iter = node->value->label_list->getFirst();
        // count amout of relevant labels in image
        while (iter) {
            if (iter->data == label && iter->used_label==true) {
                i++;
            }
            iter = node->value->label_list->getNext(iter);
        }

        i += GetTotalAmountOfLabel(label, node->right_son);

        return i;
    }

    int InOrderAddToImageAndSegmentArrays(int label, int *segments, int *images, Node<Image *, int> *node) {
        if (node == nullptr) {
            return 0;
        }
        int i = 0;
        if (node->left_son) {
            i += InOrderAddToImageAndSegmentArrays(label, segments + i, images + i, node->left_son);
        }

       // ListNode<int, int> *iter = node->value->label_list->getFirst();

        for(int j = 0; j < this->segments ; j++){
            if(node->value->segment_ptr_array[j]->data == label){
                if(!node->value->segment_ptr_array[j]->used_label){
                    continue;
                }
                segments[i] = node->value->segment_ptr_array[j]->key; // key = segment id
                images[i] = node->key; // key = imageID
                i++;
            }
        }
        i += InOrderAddToImageAndSegmentArrays(label, segments + i, images + i, node->right_son);
        //cout<<" i is "<<i<<endl;
        return i;
    }


};

class ImageTagger {
    int segments;


public:
    ImageTree *image_tree;

    //
    //constructor
    ImageTagger(int segments):segments(segments){
        image_tree=new ImageTree(segments);
    }

    //
    //
    ~ImageTagger(){
             delete (image_tree);
    }

    //
    //
    Image* FindImage(int imageID){
        Image* image_ptr = image_tree->FindValue(imageID);
        return image_ptr;
    }

    //
    //
    int GetMaxSegments(){
        return segments;
    }

    bool AddImage(int imageID){
        Image* new_image = new Image(segments,imageID);
        Node<Image*,int>* test = image_tree->Add(imageID,new_image);
        if (test != nullptr){
            return true;
        }
        delete(new_image);
        return false;
    }

    //
    //
    bool DeleteImage(int imageID){
        return image_tree->Delete(imageID);
    }

    //
    //
    bool AddLabel(int imageID, int segmentID, int label){
         /*   if(segmentID>=segments || segmentID < 0){
            return false;
        }*/
        Image* image_ptr = image_tree->FindValue(imageID);
        if(image_ptr== nullptr){
            return false;
        }

        else {
            if(image_ptr->isUsedSegment(segmentID)){
                return false;
            }else{
            // delete from unused label list
            image_ptr->unused_label_list->DeleteByPointer(image_ptr->segment_ptr_array[segmentID]);
            // add to used label list
            image_ptr->segment_ptr_array[segmentID]=image_ptr->label_list->AddElBack(label,segmentID);
            // update segment to being used
            image_ptr->segment_ptr_array[segmentID]->makeUsed();
            return true;
            }
        }
    }

    //
    //
    int GetLabel(int imageID, int segmentID) {
        if (segmentID >= segments || segmentID < 0 || imageID < 0) {
            return -1;
        }
        Image *image_ptr = image_tree->FindValue(imageID);
        if (image_ptr == nullptr) {
            return -1;
        } else {
            if (!image_ptr->isUsedSegment(segmentID)) {
                return -1;
            }
            else return image_ptr->segment_ptr_array[segmentID]->data;
        }
    }

    bool DeleteLabel(int imageID, int segmentID){
          if (segmentID >= segments || segmentID < 0 || imageID < 0) {
            return false;
        }
        Image *image_ptr = image_tree->FindValue(imageID);
        if (image_ptr == nullptr) {
            return false;
        } else{
            if (!image_ptr->isUsedSegment(segmentID)) {
                return false;
            }else{
                // delete from used label list
                image_ptr->label_list->DeleteByPointer(image_ptr->segment_ptr_array[segmentID]);
                // add to unused label list
                image_ptr->segment_ptr_array[segmentID]=image_ptr->unused_label_list->AddElBack(segmentID,segmentID);
                // update segment to being unused
                image_ptr->segment_ptr_array[segmentID]->makeUnused();
                return true;
            }

        }
    }

 };

#endif //IMAGETAGGER_IMAGETAGGER_H