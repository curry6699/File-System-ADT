// Implementation of the File System ADT

// Written by: Prithvi Sajit
// Date: 23/10/2021

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileType.h"
#include "Fs.h"
#include "utility.h"

char *path_cwd_canonical(Fs fs, char *path);
void path_cwd_canonical_helper(OF_NODE *current, char *path);
void fs_free_helper(OF_NODE *node);
OF_NODE *find_directory(Fs fs, char *path_directory);
OF_NODE *find_file(Fs fs, char *name_file, OF_NODE *parent);
void sort_insert(OF_NODE *parent, OF_NODE *node);
void fs_tree_helper_root(OF_NODE *node, OF_NODE *root);
void fs_tree_helper(OF_NODE *node, OF_NODE *directory_parent);
void fs_dl_helper_free(OF_NODE *node);
void cp_helper_insert_r(Fs fs, OF_NODE *node, OF_NODE *parent);
void cp_helper_false(Fs fs, char *src, char *dest);
void cp_helper_true_insert(Fs fs, char *src, char *dest);
char *canonical_path(OF_NODE *node, char *path);
void canonical_path_helper(OF_NODE *current, char *path);
void FsCpForMv(Fs fs, bool recursive, char *src[], char *dest);

struct FsRep {
    OF_NODE *root;
    OF_NODE *cwd;
};

Fs FsNew(void) {
    // Allocate a new struct FsRep, creating the root directory and setting it
    // as the current working directory
    Fs fs = malloc(sizeof(*fs));
    fs->root = create_new_directory("/");
    fs->cwd = fs->root;
    return fs;
}

void FsGetCwd(Fs fs, char cwd[PATH_MAX + 1]) {
    // Puts the canonical path of the current working directory into "cwd"
    char *path;
    path = malloc(sizeof(char)*PATH_MAX + 1);
    strcpy(path, "");
    path = path_cwd_canonical(fs, path);
    strcpy(cwd, path);
    free(path);
    return;
}

void FsFree(Fs fs) {
    // Function to free memory associated with fs
    if (fs == NULL) {
        return;
    }
    fs_free_helper(fs->root);
    free(fs);
}

void FsMkdir(Fs fs, char *path) {
    // Function which takes a path and makes a new directory at that path
    // Error checks for empty path, "." or ".."
    if (strlen(path) == 0) {
        printf("mkdir: cannot create directory '%s': No such file or" 
        "directory\n", path);
        return;
    }
    if (strcmp(path, ".") == 0) {
        printf("mkdir: cannot create directory '%s': File exists\n", path);
        return;
    }
    if (strcmp(path, "..") == 0) {
        printf("mkdir: cannot create directory '%s': File exists\n", path);
        return;
    }

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error checks for invalid parent directory
    if (parent_directory == NULL) {
        printf("mkdir: cannot create directory '%s': No such file or "
        "directory\n", path);
        return;
    }
    if (parent_directory->type != DIRECTORY) {
        printf("mkdir: cannot create directory '%s': Not a directory\n", path);
        return;
    }

    // Checking if the directory is nested
    if (parent_directory->child_pointer == NULL) {
        OF_NODE *new_directory = create_new_directory(file_name);
        parent_directory->child_pointer = new_directory;
        new_directory->parent_pointer = parent_directory;
    } else {
        OF_NODE *new_directory = create_new_directory(file_name);
        OF_NODE *temp = parent_directory->child_pointer;
        
        while (temp->sibling_pointer != NULL &&
            strcmp(temp->name, file_name) != 0) {
            temp = temp->sibling_pointer;
        }
        if (strcmp(temp->name, file_name) == 0) {
            printf("mkdir: cannot create directory '%s': File exists\n", path);
            free(new_directory);
            return;
        }
        sort_insert(parent_directory, new_directory);
    } 
    return;
}

void FsMkfile(Fs fs, char *path) {
    // Function which takes a path and makes a new file at that path 
    // Error checks for empty path, "." or ".."
    if (strlen(path) == 0) {
        printf("mkfile: cannot create file '%s': No such file or ", path);
        printf("directory\n"); 
        return;
    }
    if (strcmp(path, ".") == 0) {
        printf("mkfile: cannot create file '%s': File exists\n", path);
    }
    if (strcmp(path, "..") == 0) {
        printf("mkfile: cannot create file '%s': File exists\n", path);
    }
    
    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error checks for invalid parent directory
    if (parent_directory == NULL) {
        printf("mkfile: cannot create file '%s': No such file or "
        "directory\n", path);
        return;
    }
    if (parent_directory->type != DIRECTORY) {
        printf("mkfile: cannot create file '%s': Not a directory\n", path);
        return;
    }

    OF_NODE *new_file = create_new_file(file_name);

    // Checking if the directory is nested
    if (parent_directory->child_pointer == NULL) {
        parent_directory->child_pointer = new_file;
        new_file->parent_pointer = parent_directory;
    }
    else {
        OF_NODE *temp = parent_directory->child_pointer;
        
        while (temp->sibling_pointer != NULL && 
        strcmp(temp->name, file_name) != 0) {
            temp = temp->sibling_pointer;
        }
        if(strcmp(temp->name, file_name) == 0) {
            printf("mkfile: cannot create file '%s': File exists\n", path);
            free(new_file);
            return;
        }
        sort_insert(parent_directory, new_file);
        return;
    }
    return;
}

void FsCd(Fs fs, char *path) {
    // This function should change the current working directory to path
    // If path is null change the current working directory to root
    if (path == NULL) {
        fs->cwd = fs->root;
        return;
    }

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error checks for invalid parent directory
    if (parent_directory == NULL) {
        printf("cd: '%s' : No such file or directory\n", path);
        return;
    }
    if (parent_directory->type != DIRECTORY) {
        printf("cd: '%s': Not a directory\n", path);
        return;
    }
    
    OF_NODE *temp = parent_directory;
    
    // Finding file for non "." and "/"
    if (strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for invalid temp
    if (temp == NULL) {
        printf("cd: '%s': No such file or directory\n", path);
        return;
    }
    // Error check for not a directory
    if (temp->type == DIRECTORY) {
        fs->cwd = temp;
    } else {
        printf("cd: '%s': Not a directory\n", path);
        return;
    }
    return;
}

void FsLs(Fs fs, char *path) {
    // This function prints out the files' name at path in ASCII order
    // Checking for invalid path
    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    if (path == NULL) {
        char *cwd_path;
        cwd_path = malloc(sizeof(char)*PATH_MAX + 1);
        strcpy(cwd_path, "");
        cwd_path = path_cwd_canonical(fs, cwd_path);
        store_parent(cwd_path, directory_name, file_name);
        free(cwd_path);
    } else {
        store_parent(path, directory_name, file_name);
    }

    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error check for invalid directory
    if (parent_directory == NULL) {
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return;
    }
    
    OF_NODE *temp = parent_directory;

    // Error check for non directory
    if (parent_directory->type == REGULAR_FILE) {
        printf("ls: cannot access '%s': Not a directory\n", path);
        return;
    }
    // Finding file for non "." and "/"
    if (strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for invalid temp
    if (temp == NULL) {
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return;
    }
    // Print the files
    if (temp->type == DIRECTORY) {
        OF_NODE *temp2 = temp->child_pointer;
        if (temp2 == NULL){
            return;
        } 
        while(temp2 != NULL) {
            printf("%s\t", temp2->name);
            printf("\n");
            temp2 = temp2->sibling_pointer;
        }
        return;
    }
    printf("%s\n", path);
    return;
}

void FsPwd(Fs fs) {
    // This function prints the canonical path of the current working directory
    char *pwd;
    pwd = malloc(sizeof(char)*PATH_MAX + 1);
    strcpy(pwd, "");

    // Error check for invalid fs
    if (fs != NULL) {
        // Call function which gets canonical path and print
        path_cwd_canonical(fs, pwd);   
        printf("%s\n", pwd);
    }
    free(pwd);
    return;
}

void FsTree(Fs fs, char *path) {
    // Function which prints the content of a directory in a tree format
    // Checking for null path
    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    if (path == NULL) {
        path = fs->root->name;
    } 
    store_parent(path, directory_name, file_name);
    
    OF_NODE *parent_directory = find_directory(fs, directory_name);
    
    // Error check for invalid directory
    if (parent_directory == NULL) {
        printf("tree: '%s': No such file or directory\n", path);
        return;
    }
    
    OF_NODE *temp = parent_directory;

    // Error check for non directory
    if (parent_directory->type == REGULAR_FILE) {
        printf("tree: '%s': Not a directory\n", path);
        return;
    }
    // Finding file for non "." and "/"
    if(strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for invalid temp
    if (temp == NULL) {
        printf("tree: '%s': No such file or directory\n", path);
        return;
    }
    // Error check for non directory (for temp)
    if (temp->type == REGULAR_FILE) {
        printf("tree: '%s': Not a directory\n", path);
        return;
    } 
    // Print in tree format 
    printf("%s\n", path);
    if (strcmp(temp->name, "/") == 0) {
        fs_tree_helper_root(temp->child_pointer, parent_directory);
    } else {
        fs_tree_helper(temp->child_pointer, parent_directory);
    }
    return;
}

void FsPut(Fs fs, char *path, char *information) {
    // Thise function takes a path and a string, and sets the content of the
    // regular file at that path to the given string

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error check for invalid parent
    if(parent_directory == NULL) {
        printf("put: '%s': No such file or directory\n", path);
        return;
    }

    OF_NODE *temp = parent_directory;

    // Check for non directory
    if (parent_directory->type == REGULAR_FILE) {
        printf("put: '%s': Not a directory\n", path);
        return;
    }
    // Finding file for non "." and "/"
    if (strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for invalid temp
    if (temp == NULL) {
        printf("put: '%s': No such file or directory\n", path);
        return;
    }
    // Error check for non directory
    if (temp->type == DIRECTORY) {
        printf("put: '%s': Is a directory\n", file_name);
        return;
    }
    // Copy the given string into the desired file
    strcpy(temp->information, information);
    return;
}

void FsCat(Fs fs, char *path) {
    // This function takes a path and prints the content of the regular 
    // file at that path

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error check for invalid file and / or directory
    if(parent_directory == NULL) {
        printf("cat: '%s': No such file or directory\n", path);
        return;
    }
    if (parent_directory->type == REGULAR_FILE) {
        printf("cat: '%s': Not a directory\n", path);
        return;
    }

    OF_NODE *temp = parent_directory;

    // Finding file for non "." and "/"
    if (strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for valid temp and if it is a directory then print
    if (temp == NULL) {
        printf("cat: '%s': No such file or directory\n", path);
    } else if (temp->type == DIRECTORY) {
        printf("cat: '%s': Is a directory\n", file_name);
    } else if (temp->information && strcmp(temp->information, "") != 0) {
        printf("%s", temp->information);
    }
    return;
}

void FsDldir(Fs fs, char *path) {
    // This function takes a path which is expected to refer to a directory and 
    // deletes the directory if and only if it is empty

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error check for invalid file and / or directory 
    if(parent_directory == NULL) {
        printf("dldir: failed to remove '%s': No such file or ", path);
        printf("directory\n");
        return;
    }

    OF_NODE *temp = parent_directory;
    
    // Error check for non directory
    if (parent_directory->type == REGULAR_FILE) {
        printf("dldir: failed to remove '%s': Not a directory\n", path);
        return;
    }
    // Finding file for non "." and "/"
    if(strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for valid temp
    if(temp == NULL) {
        printf("dldir: failed to remove '%s': No such file or "
        "directory\n", path);
        return;
    }
    // Error check for non empty directory
    if (temp->child_pointer != NULL) {
        printf("dldir: failed to remove '%s': Directory not empty\n", path);
        return;
    }
    // Delete directory
    temp->parent_pointer->child_pointer = temp->sibling_pointer;
    free(temp);
    return;
}

void FsDl(Fs fs, bool recursive, char *path) {
    // This function takes a path and deletes the file at that path

    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    store_parent(path, directory_name, file_name);
    OF_NODE *parent_directory = find_directory(fs, directory_name);

    // Error check for invalid file and / or directory
    if(parent_directory == NULL) {
        printf("dl: cannot remove '%s': No such file or directory\n", path);
        return;
    }

    OF_NODE *temp = parent_directory;

    // Error check for non directory
    if (parent_directory->type == REGULAR_FILE) {
        printf("dl: cannot remove '%s': Not a directory\n", path);
        return;
    }
    // Finding file for non "." and "/"
    if(strcmp(file_name, ".") != 0 && strcmp(file_name, "/") != 0) {
        temp = find_file(fs, file_name, parent_directory);
    }
    // Error check for valid temp
    if(temp == NULL) {
        printf("dl: cannot remove '%s': No such file or directory\n", path);
        return;
    }
    // Error check for non regular file
    if (recursive == false && temp->type == DIRECTORY) {
        printf("dl: cannot remove '%s': Is a directory\n", path);
        return;
    }
    // Delete directory only if recursive true
    if (recursive == true && temp->type == DIRECTORY) {
        if (temp->child_pointer != NULL) {
            fs_dl_helper_free(temp->child_pointer);
        }
    }   
    temp->parent_pointer->child_pointer = temp->sibling_pointer;
    free(temp);
    return;
}

void FsCp(Fs fs, bool recursive, char *src[], char *dest) {
    // This function implements the ability to copy files
    // Check if fs is invalid
    if (fs == NULL) {
        return;
    }
    // Loop to check each file in the array "src"
    int i = 0;
    while (src[i] != NULL) {
        if (recursive == true) {
            char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
            store_parent(src[i], directory_name, file_name);
            OF_NODE *srcParent = find_directory(fs, directory_name);
            OF_NODE *file_source = find_file(fs, file_name, srcParent);

            char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
            store_parent(dest, direct_dest, name_dest);
            OF_NODE *parent_destination = find_directory(fs, direct_dest);
            OF_NODE *file_destination = find_file(fs, 
            name_dest, parent_destination);

            if (file_source->type == REGULAR_FILE) {
                strcpy(file_destination->information, file_source->information);
                return;
            }
            if (file_destination == NULL && parent_destination) {
                OF_NODE *new_directory = create_new_directory(name_dest);
                sort_insert(parent_destination, new_directory);
                if (file_source->type == DIRECTORY &&
                 file_source->child_pointer) {
                    cp_helper_insert_r(fs,
                     file_source->child_pointer, new_directory);
                } else {
                    sort_insert(new_directory, file_source);
                }
                return;
            }
            if (file_destination != NULL) {
                OF_NODE *node_new = create_new_directory(file_source->name);
                if (file_source->type == DIRECTORY &&
                 file_source->child_pointer) {
                    if (file_destination->child_pointer == NULL) {
                        file_destination->child_pointer = node_new;
                        node_new->parent_pointer = file_destination;
                    } else {
                        sort_insert(file_destination, node_new);
                    }
                    cp_helper_insert_r(fs, file_source->child_pointer,
                     node_new);
                } else {
                    file_destination->child_pointer = node_new;
                    node_new->parent_pointer = file_destination;
                }
                return;
            }
        } else {
                char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
                store_parent(src[i], directory_name, file_name);
                OF_NODE *srcParent = find_directory(fs, directory_name);
                OF_NODE *file_source = find_file(fs, file_name, srcParent);

                char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
                store_parent(dest, direct_dest, name_dest);
                OF_NODE *parent_destination = find_directory(fs, direct_dest);
                OF_NODE *file_destination = find_file(fs, name_dest,
                 parent_destination);

                if (file_destination->type == REGULAR_FILE) {
                    strcpy(file_destination->information, 
                    file_source->information);
                    return;
                }
                if (file_destination->type == DIRECTORY) {
                    if (file_destination->child_pointer) {
                        OF_NODE *temp = file_destination->child_pointer;
                        
                        while (temp->sibling_pointer!=NULL &&
                         strcmp(temp->name, file_name)!=0) {
                            temp = temp->sibling_pointer;
                        }
                        if(strcmp(temp->name, file_name)==0) {  
                            strcpy(temp->information, file_source->information);
                            return;
                        }
                        OF_NODE *node_new = create_new_file(file_name);
                        strcpy(node_new->information, file_source->information);
                        sort_insert(file_destination, node_new);

                    } else {
                        OF_NODE *node_new = create_new_file(file_name);
                        strcpy(node_new->information, file_source->information);
                        file_destination->child_pointer = node_new;
                        node_new->parent_pointer = file_destination;
                    }
                    return;
                }
                return;
        }
        i++;
    }
    return;
}
void FsMv(Fs fs, char *src[], char *dest) {
    // Not implemented
    FsCpForMv(fs, false, src, dest);

}


void FsCpForMv(Fs fs, bool recursive, char *src[], char *dest) {
    // Not implemented
    if (fs == NULL) {
        return;
    }
    int i = 0;
    while (src[i] != NULL) {
        if (recursive == true) {
            char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
            store_parent(src[i], directory_name, file_name);
            OF_NODE *srcParent = find_directory(fs, directory_name);
            OF_NODE *file_source = find_file(fs, file_name, srcParent);

            char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
            store_parent(dest, direct_dest, name_dest);
            OF_NODE *parent_destination = find_directory(fs, direct_dest);
            OF_NODE *file_destination = find_file(fs, name_dest,
             parent_destination);

            if (file_source->type == REGULAR_FILE) {
                strcpy(file_destination->information, file_source->information);
                return;
            }
            if (file_destination == NULL && parent_destination) {
                OF_NODE *new_directory = create_new_directory(name_dest);
                sort_insert(parent_destination, new_directory);
                if (file_source->type == DIRECTORY &&
                 file_source->child_pointer) {
                    cp_helper_insert_r(fs, file_source->child_pointer,
                     new_directory);
                } else {
                    sort_insert(new_directory, file_source);
                }
                return;
            }
            if (file_destination != NULL) {
                OF_NODE *node_new = create_new_directory(file_source->name);
                if (file_source->type == DIRECTORY &&
                 file_source->child_pointer) {
                    if (file_destination->child_pointer == NULL) {
                        file_destination->child_pointer = node_new;
                        node_new->parent_pointer = file_destination;
                    } else {
                        sort_insert(file_destination, node_new);
                    }
                    cp_helper_insert_r(fs, file_source->child_pointer,
                     node_new);
                } else {
                    file_destination->child_pointer = node_new;
                    node_new->parent_pointer = file_destination;
                }
                return;
            }
        } else {
                char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
                store_parent(src[i], directory_name, file_name);
                OF_NODE *srcParent = find_directory(fs, directory_name);
                OF_NODE *file_source = find_file(fs, file_name, srcParent);

                char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
                store_parent(dest, direct_dest, name_dest);
                OF_NODE *parent_destination = find_directory(fs, direct_dest);
                OF_NODE *file_destination = find_file(fs, name_dest,
                 parent_destination);

                if (file_destination->type == REGULAR_FILE) {
                    strcpy(file_destination->information,
                     file_source->information);
                    return;
                }
                if (file_destination->type == DIRECTORY) {
                    if (file_destination->child_pointer) {
                        OF_NODE *temp = file_destination->child_pointer;
                        
                        while (temp->sibling_pointer!=NULL &&
                         strcmp(temp->name, file_name)!=0) {
                            temp = temp->sibling_pointer;
                        }
                        if(strcmp(temp->name, file_name)==0) {  
                            strcpy(temp->information, file_source->information);
                            return;
                        }
                        OF_NODE *node_new = create_new_file(file_name);
                        strcpy(node_new->information, file_source->information);
                        sort_insert(file_destination, node_new);

                    } else {
                        OF_NODE *node_new = create_new_file(file_name);
                        strcpy(node_new->information, file_source->information);
                        file_destination->child_pointer = node_new;
                        node_new->parent_pointer = file_destination;
                    }
                    return;
                }
                return;
        }
        i++;
    }
    return;
}


char *path_cwd_canonical(Fs fs, char *path) {
    // This function gets the canonical current working directory
    // path and returns it
    path_cwd_canonical_helper(fs->cwd, path);
    strcat(path, fs->cwd->name);
    return path;
}

void path_cwd_canonical_helper(OF_NODE *current, char *path) {
    // This function is a helper function for path_cwd_canonical which works 
    // recursively
    // Checking for base case
    if (current == NULL) {
        return;
    }
    path_cwd_canonical_helper(current->parent_pointer, path);
    // Concatonates path recursively
    if (current->parent_pointer != NULL) {
        strcat(path, current->parent_pointer->name);
        if (strcmp(current->parent_pointer->name, "/") != 0) {
            strcat(path, "/");
        }
    }
    return;
}

void fs_free_helper(OF_NODE *node) {
    // This function is a helper function for FsFree which frees memory
    // Checking for base case
    if (node == NULL) {
        return;
    }
    fs_free_helper(node->child_pointer);
    fs_free_helper(node->sibling_pointer);
    free(node);
}

OF_NODE* find_directory(Fs fs, char *path_directory) {
    // This function finds the directory given the path_directory
    // Checking for ".", ".." and "/"
    if (strcmp(path_directory, ".") == 0) {
        return fs->cwd;
    } 
    if (strcmp(path_directory, "..") == 0) {
        if (strcmp(fs->cwd->name, "/") == 0) {
            return fs->cwd;
        }
        return fs->cwd->parent_pointer;
    } 
    if (strcmp(path_directory, "/") == 0) {
        return fs->root;
    }

    // tokenises the path_directory
    char *tokenised;
    char temp[PATH_MAX + 1];
    strcpy(temp, path_directory);
    tokenised = strtok(temp, "/");
    OF_NODE *current = fs->cwd;
    if (path_directory[0] == '/') {
        current = fs->root;
    } 

    // Loop to continually tokenise and go to next node while its valid
    while(tokenised != NULL && current != NULL) {
        if (strcmp(tokenised, "..") == 0) {
            if (current != fs->root) {
                current = current->parent_pointer;
            } 
        } 
        else if (strcmp(tokenised, ".") != 0){
            if (current->child_pointer != NULL) {
                current = current->child_pointer;
            }
            // Once the directory is found end loop
            while(current != NULL && strcmp(current->name, tokenised) != 0) {
                current = current->sibling_pointer;
            }
        }
        tokenised = strtok(NULL, "/");
    }
    return current;
}

OF_NODE *find_file(Fs fs, char *name_file, OF_NODE *parent) {
    // This function finds the file given the path_directory
    // Checking for ".", ".." and "/"
    if (strcmp(name_file, ".") == 0) {
        return parent;
    } 
    if (strcmp(name_file, "..") == 0) {
        if (parent->parent_pointer != NULL) {
            return parent->parent_pointer;
        } else {
            return parent;
        }
    } 
    if (strcmp(name_file, "/") == 0) {
        return parent;
    }

    OF_NODE *current = parent;

    // Go to next node while valid
    if (current != NULL) {
        if (current->child_pointer) {
            current = current->child_pointer;
        }
        // Once the file is found end loop
        while(current && strcmp(current->name, name_file) != 0) {
            current = current->sibling_pointer;
        }
    }
    return current;
}


void sort_insert(OF_NODE *parent, OF_NODE *node) {
    // This function is called throughout this program to continually insert and
    // sort fs

    node->parent_pointer = parent;
    OF_NODE *before = parent->child_pointer;
    OF_NODE *current = node;

    // Check ASCII order and move the files
    if (strcmp(before->name, current->name) > 0) {
        parent->child_pointer = node;
        node->sibling_pointer = before;
        return;
    } else if (before->sibling_pointer == NULL) {
        before->sibling_pointer = node; 
        return;
    }
    // Loop to keep fs in order
    while (before->sibling_pointer != NULL) {
        if (strcmp(before->name, current->name) < 0 && strcmp(current->name, 
        before->sibling_pointer->name) < 0) {
            node->sibling_pointer = before->sibling_pointer;
            before->sibling_pointer = node;
            return;
        }
        if (before->sibling_pointer->sibling_pointer == NULL) {
            node->sibling_pointer = before->sibling_pointer->sibling_pointer;
            before->sibling_pointer->sibling_pointer = node;
            return;
        }
        before = before->sibling_pointer;
    }
}

void fs_tree_helper_root(OF_NODE *node, OF_NODE *root) {
    // This function is a helper function for FsTree which prints the root
    // Check for base case
    if (node == NULL) {
        return;
    }
    
    OF_NODE *temp = node;

    // Loop to print the tab spaces
    while (temp != root) {
        printf("    ");
        temp = temp->parent_pointer;
    }
    // Print the actual node's name
    printf("%s\n", node->name);
    fs_tree_helper_root(node->child_pointer, root);
    fs_tree_helper_root(node->sibling_pointer, root);
    return;
}

void fs_tree_helper(OF_NODE *node, OF_NODE *directory_parent) {
    // This function is a helper function for FsTree which prints the files
    // except for root
    // Check for base case
    if (node == NULL) {
        return;
    } 

    OF_NODE *temp = node;

    // Loop to print the tab spaces
    while (temp->parent_pointer != directory_parent) {
        printf("    ");
        temp = temp->parent_pointer;
    }
    // Print the actual node's name
    printf("%s\n", node->name);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    fs_tree_helper(node->child_pointer, directory_parent);
    fs_tree_helper(node->sibling_pointer, directory_parent);
    return;
}

void fs_dl_helper_free(OF_NODE *node) {
    // This function is a helper function for FsDl which recursively frees
    // memory
    // Check for base case
    if (node == NULL) {
        return;
    }
    OF_NODE *temp_sibling = node->sibling_pointer;
    OF_NODE *temp_child = node->child_pointer;
    free(node);
    fs_dl_helper_free(temp_sibling);
    fs_dl_helper_free(temp_child);
    return;
}

void cp_helper_insert_r(Fs fs, OF_NODE *node, OF_NODE *parent) {
    // This function is a helper function for FsCp which recursively inserts
    // Check for base case
    if (node == NULL) {
        return;
    }
    if (parent == NULL) {
        return;
    }
    
    cp_helper_insert_r(fs, node->child_pointer, node);
    cp_helper_insert_r(fs, node->sibling_pointer, parent);

    char *path;
    char *parent_path;
    path = malloc(sizeof(char)*PATH_MAX + 1);
    parent_path = malloc(sizeof(char)*PATH_MAX + 1);
    strcpy(path, "");
    strcpy(parent_path, "");

    // Check if directory or regular file
    if (node->type == DIRECTORY) {
        cp_helper_true_insert(fs, canonical_path(node, path),
         canonical_path(parent, parent_path));
    } else {
        cp_helper_false(fs, canonical_path(node, path),
         canonical_path(parent, parent_path));
    }
    free(path);
    free(parent_path);
}

void cp_helper_false(Fs fs, char *src, char *dest) {
    // This function is a helper function for FsCp which deals with regular
    // files 
    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
    store_parent(src, directory_name, file_name);
    store_parent(dest, direct_dest, name_dest);
    OF_NODE *srcParent = find_directory(fs, directory_name);
    OF_NODE *file_source = find_file(fs, file_name, srcParent);
    OF_NODE *parent_destination = find_directory(fs, direct_dest);
    OF_NODE *file_destination = find_file(fs, name_dest, parent_destination);

    // Check for regular file
    if (file_destination->type == REGULAR_FILE) {
        strcpy(file_destination->information, file_source->information);
        return;
    }
    // Check for directory
    if (file_destination->type == DIRECTORY) {
        if (file_destination->child_pointer != NULL) {
            OF_NODE *temp = file_destination->child_pointer;
            
            // Loop to find directory
            while (temp->sibling_pointer != NULL &&
             strcmp(temp->name, file_name) !=0 ) {
                temp = temp->sibling_pointer;
            }
            // If found, copy information and return
            if(strcmp(temp->name, file_name) == 0) {  
                strcpy(temp->information, file_source->information);
                return;
            }
            // If not found create new file and copy information
            OF_NODE *node_new = create_new_file(file_name);
            strcpy(node_new->information, file_source->information);
            sort_insert(file_destination, node_new);

        } else {
            // Create new file and copy information
            OF_NODE *node_new = create_new_file(file_name);
            strcpy(node_new->information, file_source->information);
            file_destination->child_pointer = node_new;
            node_new->parent_pointer = file_destination;
        }
        return;
    }
    return;
}

void cp_helper_true_insert(Fs fs, char *src, char *dest) {
    // This function is a helper function for FsCp which deals with directories
    char directory_name[PATH_MAX + 1], file_name[PATH_MAX + 1];
    char direct_dest[PATH_MAX + 1], name_dest[PATH_MAX + 1];
    store_parent(src, directory_name, file_name);
    store_parent(dest, direct_dest, name_dest);
    OF_NODE *srcParent = find_directory(fs, directory_name);
    OF_NODE *file_source = find_file(fs, file_name, srcParent);
    OF_NODE *parent_destination = find_directory(fs, direct_dest);
    OF_NODE *file_destination = find_file(fs, name_dest, parent_destination);

    // Check if destination is non directory
    if (file_destination->type == REGULAR_FILE) {
        return;
    }
    if (file_destination->type == DIRECTORY) {
        if (file_destination->child_pointer) {
            OF_NODE *temp = file_destination->child_pointer;
            
            // Loop to find directory
            while (temp->sibling_pointer != NULL && 
            strcmp(temp->name, file_name) != 0) {
                temp = temp->sibling_pointer;
            }
            // If found, copy information and return
            if (strcmp(temp->name, file_name) == 0) {  
                strcpy(temp->information, file_source->information);
                return;
            }
            // If not found create new directory and insert
            OF_NODE *node_new = create_new_directory(file_name);
            sort_insert(file_destination, node_new);
            cp_helper_insert_r(fs, file_source->child_pointer, node_new);
        } 
        else {
            // Create new directory and insert
            OF_NODE *node_new = create_new_directory(file_name);
            file_destination->child_pointer = node_new;
            node_new->parent_pointer = file_destination;
            cp_helper_insert_r(fs, file_source->child_pointer, node_new);
        }
        return;
    }
    return;
}

char *canonical_path(OF_NODE *node, char *path) {
    // This function gets the canonical path of the given path
    // Calls its helper function
    canonical_path_helper(node, path);
    strcat(path, node->name);
    return path;
}

void canonical_path_helper(OF_NODE *current, char *path) {
    // This function is a helper function for canonical_path which recursively
    // finds the path
    // Checks base case
    if (current == NULL) {
        return;
    }
    canonical_path_helper(current->parent_pointer, path);
    // Concatonates the path
    if (current->parent_pointer != NULL) {
        strcat(path, current->parent_pointer->name);
        if (strcmp(current->parent_pointer->name, "/") != 0) {
            strcat(path, "/");
        }
    }
    return;
}