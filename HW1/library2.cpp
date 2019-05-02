//
// Created by Yair Shachar on 13/12/2018.
//
#include "library2.h"
#include "ImageTagger.h"

/* Required Interface for the Data Structure
 * -----------------------------------------*/

/* Description:   Initiates the data structure.
 * Input:         segments - The number of segments in each picture.
 * Output:        DS - A pointer to the data structure.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void *Init(int segments) {
    if (segments <= 0) {
        return NULL;
    }
    try {
        ImageTagger *DS = new ImageTagger(segments);
        return (void *) DS;
    }
    catch (std::bad_alloc &ba) {
        return nullptr;
    }
}

/* Description:   Adds a new image to the data structure.
 * Input:         DS - A pointer to the data structure.
 *                image - The image to add.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or imageID <= 0.
 *                FAILURE - If the image is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType AddImage(void *DS, int imageID){
    if (imageID<=0 || DS == NULL){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;
    try{
        if(image_DS->AddImage(imageID)){
        return SUCCESS;
        }
               return FAILURE;
    }
    catch (const std::bad_alloc &ba){
        return ALLOCATION_ERROR;
    }
}

/* Description:   Deletes an image from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                image - The image to remove.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or imageID <= 0.
 *                FAILURE - If the image does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType DeleteImage(void *DS, int imageID){
    if(DS==NULL || imageID <=0){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;
    if(image_DS->DeleteImage(imageID)){
        return SUCCESS;
    } else return FAILURE;
}

/* Description:   Tags the a segment in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to tag.
 * 				  segmentID - The segment to tag.
 * 				  label - The label of the segment.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL segmentID >= segments or segmentID < 0 or imageID < 0 or label < 0.
 *                FAILURE - If the image does not exist in the DS, or the segment is allready tagged.
 *                SUCCESS - Otherwise.
 */
StatusType AddLabel(void *DS, int imageID, int segmentID, int label){
    if(DS==NULL || imageID<=0 || label<=0 || segmentID<0){
        return INVALID_INPUT;
    }

    ImageTagger* image_DS = (ImageTagger*)DS;
    if(segmentID>=image_DS->GetMaxSegments()){
        return INVALID_INPUT;
    }
    try{
        if(image_DS->AddLabel(imageID,segmentID,label)){
            return SUCCESS;
        }   return FAILURE;
    }     catch (const std::bad_alloc &ba){
        return ALLOCATION_ERROR;
    }


}

/* Description:   Gets the a label of a segment in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to get the label from.
 * 				  segmentID - The segment to get the label from.
 * 				  label - A pointer to a variable which will hold the label.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL segmentID >= segments or segmentID < 0 or imageID < 0 or label == NULL.
 *                FAILURE - If the image does not exist in the DS, or the segment is not tagged.
 *                SUCCESS - Otherwise.
 */
StatusType GetLabel(void *DS, int imageID, int segmentID, int *label){
    if(DS==NULL || imageID<=0 || label==NULL || segmentID<0){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;
    if(segmentID>=image_DS->GetMaxSegments()){
        return INVALID_INPUT;
    }
    int templabel=image_DS->GetLabel(imageID,segmentID);
    if(templabel==-1){
        return FAILURE;
    }
    else *label=templabel;
    return SUCCESS;
}

/* Description:   Deletes the a label of a segment in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to delete the label from.
 * 				  segmentID - The segment to delete the label from.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL segmentID >= segments or segmentID < 0 or imageID < 0.
 *                FAILURE - If the image does not exist in the DS, or the segment is not tagged.
 *                SUCCESS - Otherwise.
 */
StatusType DeleteLabel(void *DS, int imageID, int segmentID){
    if(DS==NULL || imageID<=0 || segmentID<0){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;
    if(segmentID>=image_DS->GetMaxSegments()){
        return INVALID_INPUT;
    }
    if(image_DS->DeleteLabel(imageID,segmentID)){
        return SUCCESS;
    } else return FAILURE;
}

/* Description:   Returns a list of all the unlabeled segments in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to get the segments from.
 * Output:        segments - A pointer to an array which will hold the unlabled segments. Note that you need to allocate the array inside the function.
 *  		      numOfSegments - A pointer to a variable which will hold the number of unlabled segments.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or imageID <= 0 or segments == NULL or numOfSegments == NULL.
 *                FAILURE - If the image does not exist in the DS, or the there is no unlabeled segment in the image.
 *                SUCCESS - Otherwise.
 */
StatusType GetAllUnLabeledSegments(void *DS, int imageID, int **segments, int *numOfSegments){
    if(DS==NULL || imageID<=0 || segments== NULL || numOfSegments == NULL){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;

    //Retrieve relevant image.
    Image* image_ptr=image_DS->FindImage(imageID);
    if(image_ptr== nullptr){
        return FAILURE;
    }

    int num_of_seg=image_ptr->unused_label_list->Size();
    if(num_of_seg==0){
        *segments=NULL;
        *numOfSegments=0;
        return FAILURE;
    }
    *numOfSegments=num_of_seg;
    *segments= (int*)malloc((*numOfSegments)*sizeof(int)) ;
     if(*segments==NULL){
        return ALLOCATION_ERROR;
    }
    ListNode<int,int>* iter = image_ptr->unused_label_list->getFirst();
    int i=0;
    while(iter) {
        (*segments)[i] = iter->data;
        iter=image_ptr->unused_label_list->getNext(iter);
        i++;
    }
    return SUCCESS;
    }

/* Description:   Returns a list of all the segments tagged with a certain label.
 * Input:         DS - A pointer to the data structure.
 *                label - The label to lookup for.
 * Output:        images - A pointer to an array which will hold the IDs of the images containing segments tagged with label. Note that you need to allocate the array inside the function.
 *                segments - A pointer to an array which will hold the segments of the images containing segments tagged with label. Note that you need to allocate the array inside the function.
 *  		      numOfSegments - A pointer to a variable which will hold the number of returned segments.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If one of the pointers is NULL or label < 0.
 *                SUCCESS - Otherwise.
 */
StatusType GetAllSegmentsByLabel(void *DS, int label, int **images, int **segments, int *numOfSegments){
   if(DS==NULL || images==NULL || segments == NULL || numOfSegments == NULL || label<=0){
        return INVALID_INPUT;
    }
    ImageTagger* image_DS = (ImageTagger*)DS;

    *numOfSegments=image_DS->image_tree->GetTotalAmountOfLabel(label,image_DS->image_tree->root);
    if(*numOfSegments==0){
        *segments=NULL;
        *images=NULL;
        return SUCCESS;
    }
    *segments= (int*)malloc((*numOfSegments)*sizeof(int));
    *images= (int*)malloc((*numOfSegments)*sizeof(int));
    if(*images==NULL || *segments==NULL){
        return ALLOCATION_ERROR;
    }
    image_DS->image_tree->InOrderAddToImageAndSegmentArrays(label,*segments,*images,image_DS->image_tree->root);
    return SUCCESS;

};

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */

void Quit(void** DS) {
    if (*DS == NULL) {
        return;
    }
    ImageTagger* image_DS = (ImageTagger*)*DS;
    delete (image_DS);
    *DS=NULL;
};
