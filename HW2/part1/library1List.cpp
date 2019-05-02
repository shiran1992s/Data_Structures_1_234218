//
// Created by shira on 05/12/2018.
//

#include "library1.h"
#include "DictListFinale.h"




void* Init(){
    try {
        mapList<void *, int> *DS = new mapList<void *, int>();
        return (void *) DS;
    }
    catch (std::bad_alloc &ba) {
        return nullptr;
    }
}
StatusType Add(void *DS, int key, void* value, void** node){
    if(!DS || !value){
        return INVALID_INPUT;
    }else{
        mapList<void*,int>* dictionary = (mapList<void*,int>*)DS;
       try {
           *node = (void *) dictionary->Add(value, key);
       }
       catch (std::bad_alloc &ba) {
           return ALLOCATION_ERROR;
       }
    }
    if(!(*node)){
        return FAILURE;
    }
    return SUCCESS;
}
StatusType Find(void *DS, int key, void** value){
    if(!DS || !value){
        return INVALID_INPUT;
    }else{
        mapList<void*,int>* dictionary = (mapList<void*,int>*)DS;
        *value = (void*)dictionary->Find(key);
    }
    if(!(*value)){
        return FAILURE;
    }
    return SUCCESS;
}
StatusType Delete(void *DS, int key){
    if(!DS ) {
        return INVALID_INPUT;
    }
    mapList<void*,int>* dictionary = (mapList<void*,int>*)DS;
    bool done = dictionary->Delete(key);
    if(done){
        return SUCCESS;
    }
    return FAILURE;
}
StatusType DeleteByPointer(void *DS, void* p){
    if(!DS || p ) {
        return INVALID_INPUT;
    }
    mapList<void*,int>* dictionary = (mapList<void*,int>*)DS;
    bool done = dictionary->DeleteByPointer((ListNode<void*,int>*)p);
    if(done){
        return SUCCESS;
    }
    return FAILURE;
}
StatusType Size(void *DS, int *n){
    if(!DS || !n){
        return INVALID_INPUT;
    }else{
        mapList<void*,int>* dictionary = (mapList<void*,int>*)DS;
        *n = dictionary->Size();
    }
    return SUCCESS;
}
void Quit(void** DS){
    delete *((mapList<void*,int>**)DS);
    *DS = nullptr;
}