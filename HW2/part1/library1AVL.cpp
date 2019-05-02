//
// Created by Yair Shachar on 11/12/2018.
//
#include "AVLtree.h"
#include "library1.h"

/* Required Interface for the Data Structure
 * -----------------------------------------*/

/* Description:   Initiates the data structure.
 * Input:         None.
 * Output:        DS - A pointer to the data structure.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void *Init(){
    return (void*)new AVLtree<void*,int>;
}

/* Description:   Adds a new item to the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The key to add.
 *                value - A pointer to the value to add.
 				  node - A pointer to a place which will hold a pointer to the new node in the DS.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType Add(void *DS, int key, void* value, void** node){
    if(DS== NULL){
        return INVALID_INPUT;
    }
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)DS;
    try{*node = (void*)map->Add(key,value);}
    catch(std::bad_alloc &ba){
        return ALLOCATION_ERROR;
    }
    if(*node){
        return SUCCESS;
    } return FAILURE;
}

/* Description:   Finds an item in the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The item to find.
 * Output:        value - a pointer to a variable which will contain the value of the element.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - If the item is found in the DS.
 */
StatusType Find(void *DS, int key, void** value){
    if(DS == NULL){
        return INVALID_INPUT;
    }
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)DS;
    Node<void*,int>* node = map->Find(map->GetRootNode(),key);
    if(node==NULL){
        return FAILURE;
    }
    *value= node->value;
    return SUCCESS;
}

/* Description:   Delete an item from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The item to delete.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType Delete(void *DS, int key){
    if(DS == NULL){
        return INVALID_INPUT;
    }
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)DS;
    Node<void*,int>* node = map->Find(map->GetRootNode(),key);
    if(nullptr==node){
        return FAILURE;
    }
    if(map->Delete(key)){
        return SUCCESS;
    } return FAILURE;
}

/* Description:   Delete an item from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                p - A pointer to the node to delete.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType DeleteByPointer(void *DS, void* p){
    if(DS==NULL || p==NULL){
        return INVALID_INPUT;
    }
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)DS;
    if(map->DeleteByPointer((Node<void*,int>*)p)){
        return SUCCESS;
    }return FAILURE;
}

/* Description:   Returns the current number of items in the data structure.
 * Input:         DS - A pointer to the data structure.
 * Output:        n - A pointer to a variable that should be updated to the number of items in the data structure.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                SUCCESS - Otherwise.
 */
StatusType Size(void *DS, int *n){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)DS;
    *n=map->Size();
    return SUCCESS;
}

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void Quit(void** DS){
    AVLtree<void*,int>* map = (AVLtree<void*,int>*)*DS;
    map->Quit();
}
