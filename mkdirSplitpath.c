#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    char baseName[64], dirName[128];

    // Handle empty or missing argument
    if (strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
        if (strlen(pathName) == 0)
            printf("MKDIR ERROR: no path provided\n");
        else
            printf("MKDIR ERROR: cannot create root directory\n");
        return;
    }

    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (!parent) {
        printf("ERROR: directory %s does not exist\n", dirName);
        return;
    }

    // Check if directory already exists
    struct NODE* child = parent->childPtr;
    while (child) {
        if (strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    // Create and link new directory
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    if (!parent->childPtr)
        parent->childPtr = newDir;
    else {
        struct NODE* temp = parent->childPtr;
        while (temp->siblingPtr)
            temp = temp->siblingPtr;
        temp->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}


//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    char temp[128];
    strcpy(temp, pathName);

    char* lastSlash = strrchr(temp, '/');
    if (lastSlash) {
        *lastSlash = '\0';
        strcpy(baseName, lastSlash + 1);
        strcpy(dirName, temp);
    } else {
        strcpy(baseName, temp);
        strcpy(dirName, "");
    }

    struct NODE* current = (pathName[0] == '/') ? root : cwd;

    if (strlen(dirName) == 0)
        return current;

    char pathCopy[128];
    strcpy(pathCopy, dirName);
    char* token = strtok(pathCopy, "/");

    while (token && current) {
        struct NODE* child = current->childPtr;
        while (child && strcmp(child->name, token) != 0)
            child = child->siblingPtr;

        if (!child || child->fileType != 'D')
            return NULL;

        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}