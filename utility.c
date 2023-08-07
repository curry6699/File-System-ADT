// Written by: Prithvi Sajit 
// Date: 23/10/2021

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "FileType.h"
#include "utility.h"
#include "Fs.h"

// Create a directory node
OF_NODE* create_new_directory(char name[]) {
    OF_NODE *dir = (OF_NODE*)malloc(sizeof(OF_NODE));
    dir->type = DIRECTORY;
    dir->parent_pointer = NULL;
    dir->child_pointer = NULL;
    dir->sibling_pointer = NULL;
    strcpy(dir->name, name);
    strcpy(dir->information, "");
    return dir;
}

// Create a file node
OF_NODE* create_new_file(char name[]) {
    OF_NODE *file = (OF_NODE*)malloc(sizeof(OF_NODE));
    file->type = REGULAR_FILE;
    file->parent_pointer = NULL;
    file->child_pointer = NULL;
    file->sibling_pointer = NULL;
    strcpy(file->name, name);
    strcpy(file->information, "");
    return file;
}

// Store path of the node's parent in path
void store_parent(char *path, char *dir_name, char *file_name) {
    char temp[PATH_MAX], temp2[PATH_MAX];
    strcpy(temp, path);
    strcpy(temp2, path);
    strcpy(dir_name, dirname(temp));
    strcpy(file_name, basename(temp2));
}

