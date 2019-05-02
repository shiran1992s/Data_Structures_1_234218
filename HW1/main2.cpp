/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2018-2019                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main2.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADDIMAGE_CMD = 1,
    DELETEIMAGE_CMD = 2,
    ADDLABEL_CMD = 3,
    GETLABEL_CMD = 4,
    DELETELABEL_CMD = 5,
    GETALLUNLABELEDSEGMENTS_CMD = 6,
    GETALLSEGMENTSSBYLABEL_CMD = 7,
    QUIT_CMD = 8
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
        "init",
        "addImage",
        "deleteImage",
        "addLabel",
        "getLabel",
        "deleteLabel",
        "getAllUnLabeledSegments",
        "getAllSegmentsByLabel",
        "quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddImage(void* DS, const char* const command);
static errorType OnDeleteImage(void* DS, const char* const command);
static errorType OnAddLabel(void* DS, const char* const command);
static errorType OnGetLabel(void* DS, const char* const command);
static errorType OnDeleteLabel(void* DS, const char* const command);
static errorType OnGetAllUnlabeledSegments(void* DS, const char* const command);
static errorType OnGetAllSegmentssByLabel(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADDIMAGE_CMD):
            rtn_val = OnAddImage(DS, command_args);
            break;
        case (DELETEIMAGE_CMD):
            rtn_val = OnDeleteImage(DS, command_args);
            break;
        case (ADDLABEL_CMD):
            rtn_val = OnAddLabel(DS, command_args);
            break;
        case (GETLABEL_CMD):
            rtn_val = OnGetLabel(DS, command_args);
            break;
        case (DELETELABEL_CMD):
            rtn_val = OnDeleteLabel(DS, command_args);
            break;
        case (GETALLUNLABELEDSEGMENTS_CMD):
            rtn_val = OnGetAllUnlabeledSegments(DS, command_args);
            break;
        case (GETALLSEGMENTSSBYLABEL_CMD):
            rtn_val = OnGetAllSegmentssByLabel(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("init was already called.\n");
        return (error_free);
    };
    isInit = true;

    int hours;
    ValidateRead(sscanf(command, "%d", &hours), 1, "%s failed.\n", commandStr[INIT_CMD]);
    *DS = Init(hours);

    if (*DS == NULL) {
        printf("init failed.\n");
        return error;
    };

    printf("init done.\n");
    return error_free;
}


/***************************************************************************/
/* OnAddImage                                                               */
/***************************************************************************/
static errorType OnAddImage(void* DS, const char* const command) {
    int imageID;
    ValidateRead(sscanf(command, "%d", &imageID), 1, "%s failed.\n", commandStr[ADDIMAGE_CMD]);
    StatusType res = AddImage(DS, imageID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDIMAGE_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDIMAGE_CMD], ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnDeleteImage                                                            */
/***************************************************************************/
static errorType OnDeleteImage(void* DS, const char* const command) {
    int imageID;
    ValidateRead(sscanf(command, "%d", &imageID), 1, "%s failed.\n", commandStr[DELETEIMAGE_CMD]);
    StatusType res = DeleteImage(DS, imageID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[DELETEIMAGE_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[DELETEIMAGE_CMD], ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnAddLabel                                                            */
/***************************************************************************/
static errorType OnAddLabel(void* DS, const char* const command) {
    int imageID, segmentID, courseID;
    ValidateRead(sscanf(command, "%d %d %d", &imageID, &segmentID, &courseID), 3, "%s failed.\n", commandStr[ADDLABEL_CMD]);
    StatusType res = AddLabel(DS, imageID, segmentID, courseID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDLABEL_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDLABEL_CMD], ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnGetLabel                                                            */
/***************************************************************************/
static errorType OnGetLabel(void* DS, const char* const command) {
    int imageID, segmentID, courseID;
    ValidateRead(sscanf(command, "%d %d", &imageID, &segmentID), 2, "%s failed.\n", commandStr[GETLABEL_CMD]);
    StatusType res = GetLabel(DS, imageID, segmentID, &courseID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[GETLABEL_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %d\n", commandStr[GETLABEL_CMD], courseID);
    return error_free;
}

/***************************************************************************/
/* OnDeleteLabel                                                         */
/***************************************************************************/
static errorType OnDeleteLabel(void* DS, const char* const command) {
    int imageID, segmentID;
    ValidateRead(sscanf(command, "%d %d", &imageID, &segmentID), 2, "%s failed.\n", commandStr[DELETELABEL_CMD]);
    StatusType res = DeleteLabel(DS, imageID, segmentID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[DELETELABEL_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[DELETELABEL_CMD], ReturnValToStr(res));
    return error_free;
}

int CompareInt(const void* a, const void* b)
{
    if (*(int*)a == *(int*)b) return 0;
    return *(int*)a < *(int*)b ? -1 : 1;
}

/***************************************************************************/
/* OnGetAllUnlabeledSegments                                                       */
/***************************************************************************/
static errorType OnGetAllUnlabeledSegments(void* DS, const char* const command) {
    int imageID, numOfSegments;
    int *segments;
    ValidateRead(sscanf(command, "%d", &imageID), 1, "%s failed.\n", commandStr[GETALLUNLABELEDSEGMENTS_CMD]);
    StatusType res = GetAllUnLabeledSegments(DS, imageID, &segments, &numOfSegments);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[GETALLUNLABELEDSEGMENTS_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[GETALLUNLABELEDSEGMENTS_CMD], ReturnValToStr(res));

    if (numOfSegments > 0) {
        //Sort the values
        qsort(segments, numOfSegments, sizeof(int), CompareInt);
    }

    for (int i = 0; i < numOfSegments; i++)
    {
        printf("%d\n", segments[i]);
    }

    printf("--End of segments list--\n");

    free(segments);
    return error_free;
}

/***************************************************************************/
/* OnGetAllSegmentssByLabel                                              */
/***************************************************************************/
static errorType OnGetAllSegmentssByLabel(void* DS, const char* const command) {
    int courseID, numOfLabels;
    int *images, *segments;
    ValidateRead(sscanf(command, "%d", &courseID), 1, "%s failed.\n", commandStr[GETALLSEGMENTSSBYLABEL_CMD]);
    StatusType res = GetAllSegmentsByLabel(DS, courseID, &images, &segments, &numOfLabels);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[GETALLSEGMENTSSBYLABEL_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[GETALLSEGMENTSSBYLABEL_CMD], ReturnValToStr(res));
    if (numOfLabels > 0) {
        printf("Image\t||\tSegment\n");
    }

    for (int i = 0; i < numOfLabels; i++)
    {
        printf("%d\t||\t%d\n", images[i], segments[i]);
    }

    printf("--End of segments list--\n");

    free(images);
    free(segments);

    return error_free;
}

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("quit failed.\n");
        return error;
    };

    isInit = false;
    printf("quit done.\n");
    return error_free;
}

#ifdef __cplusplus
}
#endif
