// Written by: Prithvi Sajit 
// Date: 23/10/2021

#ifndef UTILITY_H
#define UTILITY_H

#include "FileType.h"
#include "Fs.h"

void store_parent(char *path, char *dir_name, char *file_name);

typedef struct node {
    FileType type;           
    char name[PATH_MAX], information[PATH_MAX];    
    struct node *child_pointer, *sibling_pointer, *parent_pointer;        
} OF_NODE;

OF_NODE* create_new_file(char name[]);
OF_NODE* create_new_directory(char name[]);

#endif