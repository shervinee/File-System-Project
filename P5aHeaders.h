//
//  main.h
//  Program5
//
//  Created by shervin shahidizandi on 5/4/16.
//
//

#ifndef main_h
#define main_h

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>


/* to store the hidden file detail into a Binary Search Tree */
typedef struct FileN
{
    char *name;
    time_t lastModif;
    ssize_t size;
    struct FileN* leftChild;
    struct FileN* rightChild;
}FileN;

FileN* createNode(off_t key, const char* name, time_t modifTime);//for crating a node
FileN* insert(struct FileN* root, off_t key, const char* name, time_t modifTime);//to insert
void* checkedMalloc(size_t size);//for checking malloc
FileN *findHiddenFiles(const char filePath[]);


#endif /* main_h */
