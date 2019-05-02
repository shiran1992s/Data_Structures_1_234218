//
// Created by shira on 07/01/2019.
//

#ifndef WET2_STATICEYE_H
#define WET2_STATICEYE_H

#include "HashTableAlpha.h"


class StaticEye {
    ImageHashTable system;
public:
    StaticEye(int num_pixels):system(num_pixels){}
    //StaticEye(const StaticEye& se):pixels(se.pixels),im_table(se.im_table){}
    ~StaticEye()= default;
    bool AddImage(int im_id){
        return system.AddImage(im_id);
    }
    bool DeleteImage(int im_id){
        return system.DeleteImage(im_id);
    }
    bool SetLabelScore(int imageID, int pixel, int label, int score){
        return system.SetLabelScore(imageID,pixel,label,score);
    }
    bool ResetLabelScore(int imageID, int pixel, int label){
        return system.ResetLabelScore(imageID,pixel,label);
    }
    int GetHighestScoredLabel(int imageID, int pixel, int *label){
        return system.GetHighestScoredLabel(imageID,pixel,*label);
    }
    bool MergeSuperPixels(int image_id, int pixel1, int pixel2){
        return system.MergeSuperPixels(image_id,pixel1,pixel2);
    }
    int GetMaxPixels(){
        return system.GetMaxPixels();
    }
};


#endif //WET2_STATICEYE_H
