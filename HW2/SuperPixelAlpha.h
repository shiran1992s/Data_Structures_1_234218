//
// Created by Yair Shachar on 09/01/2019.
//

#ifndef STATICEYE_SUPERPIXELALPHA_H
#define STATICEYE_SUPERPIXELALPHA_H

#include "ScoreRankedAVLtree.h"
#include <stdio.h>
#include <iostream>
using std::cout;
class SuperPixelNode {
public:

    int super_pixel_size;
    int super_pixel_id;

    SuperPixelNode* father;

    AVLtree<int,int>* labels;
    //
    //Constructor
    SuperPixelNode(int i):super_pixel_size(1),super_pixel_id(i),father(nullptr){
        labels= new AVLtree<int,int>();
    }
    ~SuperPixelNode(){
        delete labels;
    }
    //
    //SetLabelScore
    bool AddLabelScore(int label, int score){
        if(labels== nullptr){
            return false;
        }
        return(labels->Add(label,label,score));
    }
    bool RemoveLabelScore(int label){
        if(labels== nullptr){
            return false;
        }
        return(labels->Delete(label));
    }

    //
    //merges sp2 into current pixel
    // after this process sp2's tree will be deleted. and its size will be 0.
    // sp1 size will be +1 and will have the united label tree of the two pixels
    void MergePixels(SuperPixelNode& sp2){
           AVLtree<int,int>* merged_label_tree=MergeTrees(this->labels,sp2.labels);
    if(merged_label_tree != nullptr){
    if(sp2.labels != nullptr){
        sp2.labels->PostOrderDelete(sp2.labels->root);
        sp2.labels->size_of_tree=0;
        sp2.labels->root= nullptr;
    }
      if(this->labels != nullptr){
          delete this->labels;
          this->labels = nullptr;
          //this->labels->PostOrderDelete(this->labels->root);
      }
      this->labels=merged_label_tree;
    }
        this->super_pixel_size++;
        sp2.super_pixel_size=-1;
    }

    void PrintPixel(){
        std::cout<<"*Printing pixel #"<<this->super_pixel_id<<"*"<<std::endl;
        std::cout<<"size: "<<this->super_pixel_size<<std::endl;
        std::cout<<"labels: "<<std::endl;
        this->labels->PrintTree();
    }
};

#endif //STATICEYE_SUPERPIXELALPHA_H
