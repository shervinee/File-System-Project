//
//  helperFunctions.c
//  Program5
//
//  Created by shervin shahidizandi on 5/5/16.
//
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include"P5aHeaders.h"
void* checkedMalloc(size_t size)
{
    void* buffer = malloc(size);
    if(buffer == NULL)
    {
        fprintf(stderr,"malloc error");
        exit(1);
    }
    return buffer;
}

FileN* createNode(off_t key, const char* name, time_t modifTime)
{
    struct FileN* tempNode = checkedMalloc(sizeof(struct FileN));
    tempNode->name = (char*)malloc(strlen(name) +1);
    strcpy(tempNode->name,name);
    tempNode-> size = key;
    tempNode-> leftChild == NULL;
    tempNode-> rightChild == NULL;
    
    return tempNode;
}

FileN* insert(struct FileN* root, off_t key, const char* name, time_t modifTime)
{
    if(root == NULL)
    {
        return createNode(key,name,modifTime);
    }
    if(root->size < key)
    {
        root-> leftChild = insert(root->leftChild, key, name, modifTime);
    }
    if(root->size > key)
    {
        root-> rightChild = insert(root->rightChild, key, name, modifTime);
    }
    return root;
}
void PrintIn(FileN* root)
{
   if(root != NULL)
    {
        char time[64];
        PrintIn(root->leftChild);
        struct tm *now = localtime(&root->lastModif);
        strftime(time, sizeof(time), "%m/%d/%Y %H:%M:%S", now);//getting the last modification in the right format
        printf("%-20s  %-12zu, %-20s\n", root->name, root->size, root->lastModif);
        PrintIn(root->rightChild);
    
    }
}