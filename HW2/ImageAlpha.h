//
// Created by Yair Shachar on 12/01/2019.
//

#ifndef STATICEYE2_IMAGEALPHA_H
#define STATICEYE2_IMAGEALPHA_H

#include "UnionFind.h"
class ImageAlpha{
    int image_id;
    int max_pixels;
    UnionFind* super_pixels_uf;
public:

    ImageAlpha(int image_id_input, int max_pixels_input){
        this->image_id=image_id_input;
        this->max_pixels=max_pixels_input;
        super_pixels_uf = new UnionFind(max_pixels);
        for(int i=0 ; i<max_pixels_input ; i++){
            super_pixels_uf->MakeSet(i);
        }
    }

    ~ImageAlpha(){
        delete super_pixels_uf;
    }

    SuperPixelNode* GetPixel(int pixel_id){
        return super_pixels_uf->GetPixel(pixel_id);
    }

    bool SetLabelScore(int pixel, int label, int score){
        SuperPixelNode* sp_ptr = super_pixels_uf->GetPixel(pixel);
        return sp_ptr->AddLabelScore(label,score);
    }

    bool ResetLabelScore(int pixel, int label){
        SuperPixelNode* sp_ptr = super_pixels_uf->GetPixel(pixel);
        return sp_ptr->RemoveLabelScore(label);
    }

    int GetHighestScoredLabel(int pixel){
        SuperPixelNode* sp_ptr = super_pixels_uf->GetPixel(pixel);
        if(sp_ptr->labels== nullptr){
            return -1;
        }
        if(sp_ptr->labels->root== nullptr){
            return -1;
        }
        return sp_ptr->labels->root->max_scored_label;
    }
    bool MergeSuperPixels(int pixel1, int pixel2){
        return super_pixels_uf->Union(super_pixels_uf->Find(pixel1),super_pixels_uf->Find(pixel2));
    }

};



#endif //STATICEYE2_IMAGEALPHA_H
