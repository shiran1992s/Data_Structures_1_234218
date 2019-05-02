//
// Created by shira on 07/01/2019.
//

#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "SuperPixelAlpha.h"

//template <class T>
class UnionFind {
    int size;
    int* up_tree;
    int* groups;
    SuperPixelNode** pixels;


    void Zip(int pixel_id, int root){
        while(up_tree[pixel_id] != root){
            int tmp = pixel_id;
            pixel_id = up_tree[pixel_id];
            up_tree[tmp]=root;
        }
        if(up_tree[pixel_id] == root)
        return;
    }
public:
    UnionFind(int size_inp):size(size_inp),up_tree(nullptr),groups(nullptr),pixels(nullptr){
        up_tree = new int[size];
        groups = new int[size];
        pixels = new SuperPixelNode*[size];
        for(int i=0 ; i<size ; i++){
            pixels[i]= nullptr;
        }
    }
    UnionFind(const UnionFind& un):size(un.size),up_tree(nullptr),groups(nullptr),pixels(nullptr){ // dont sure we need
        up_tree = new int[size];
        groups = new int[size];
        pixels = new SuperPixelNode*[size];
        for(int i=0;i<size;i++){
            up_tree[i] = un.up_tree[i];
            groups[i] = un.groups[i];
            pixels[i] = un.pixels[i];
        }
    }
    ~UnionFind(){
        if(up_tree){
            delete[] up_tree;
            up_tree= nullptr;
        }
        if(groups){
            delete[] groups;
            groups= nullptr;
        }
        if(pixels){
            for (int i= 0; i<size; i++){
                if(pixels[i] != nullptr){
                    delete pixels[i];
                }
            }
            delete[] pixels;
            pixels= nullptr;
        }
    }
    UnionFind& operator=(const UnionFind& un){
      int*  up_tree_tmp = new int[un.size];
      int* groups_tmp = new int[un.size];
        SuperPixelNode** pixels_tmp = new SuperPixelNode*[un.size];
        if(up_tree){
            delete[] up_tree;
            up_tree= up_tree_tmp;
        }else{
            up_tree= up_tree_tmp;
        }
        if(groups){
            delete[] groups;
            groups= groups_tmp;
        }else{
            groups= groups_tmp;
        }
        if(pixels){
            delete[] pixels;
            pixels= pixels_tmp;
        }else{
            pixels= pixels_tmp;
        }
        for(int i=0;i<un.size;i++){
            up_tree[i] = un.up_tree[i];
            groups[i] = un.groups[i];
            pixels[i] = un.pixels[i];
        }
        size = un.size;
        return *this;
    }
    void MakeSet(int pixel_id){
        SuperPixelNode* new_sp = new SuperPixelNode(pixel_id);
        groups[pixel_id] = 1;
        pixels[pixel_id] = new_sp;
        up_tree[pixel_id] = pixel_id;
    }
    int Find(int pixel_id){
        int tmp = pixel_id;
        if(up_tree[pixel_id] == pixel_id ){
            return pixel_id;
        }else{
            while(up_tree[pixel_id] != pixel_id){
                pixel_id = up_tree[pixel_id];
            }
            Zip(tmp,pixel_id);
            return pixel_id;
        }
    }

    SuperPixelNode* GetPixel(int pixel_id){
        int pixel_group=Find(pixel_id);
        return pixels[pixel_group];
    }

    bool Union(int group1 ,int group2){
        if(group1<0 || group2 < 0 || group1 > size || group2 > size){
            return false;
        }
        SuperPixelNode* el1 = pixels[group1];
        SuperPixelNode* el2 = pixels[group2];
        if(el1==el2){
            return false;
        }
        if (groups[group1] >= groups[group2]){
            el1->MergePixels(*el2);
            up_tree[group2]=group1;
            groups[group1]+=groups[group2];
            groups[group2]=0;
            return true;
        }else{
            el2->MergePixels(*el1);
            up_tree[group1]=group2;
            groups[group2]+=groups[group1];
            groups[group1]=0;
            return true;
        }
    }

    void UnPrint(){
        int counter =0;
        cout<<"This is up_tree array:\n";
        for (int i=0;i<size;i++){
            cout<<"up_tree element: "<<i+1<<" father is: "<<up_tree[i]+1;
            cout<<"\n";
        }
        cout<<"\n";
        for (int i=0;i<size;i++){
            cout<<"Pixel num:"<<i+1<<" is in Group:"<<up_tree[i]+1<<"\n";
            cout<<"Group num:"<<i+1<<" has size:"<<groups[i]<<"\n";
            if(groups[i] != 0){
                counter++;
            }
        }
        cout<<"Total groups num is:"<<counter<<"\n";
    }


};


#endif //WET2_UNIONFIND_H
