#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

typedef struct PathInfo {
    char absolute_path[512];
    char relative_folder[256];
    char file_name[32];
    struct PathInfo *next;
} FilePathInfo;

#define TH_FOLDER ".th"
#define COMMIT_FOLDER "commits"

#define LOG_FILE "structure_log"
#define COMMIT_FILE "commit_log"
#define USER_FILE "user_info"

#define MAXLEN 1024
#define DELIM " "

// Get folder structure-tree
void folder_structure(char *root_path, FILE **fo);
// Check if input path is a directory
int isDirectory(const char *path);
// Check if the directory is valid (../ and ./ is invalid)
int isValidDirectory(const char *path);
// Tokenize a string into tokens
void strTokenize(char *string, char strArr[10][MAXLEN], char *delim);
// Get user info
void getUserInfo(char *filename, char *username, char *email);
// Create a folder with absolute path
void create_path(char* absolute_path);
// Get folder structure
void get_folder_structure(char *root_path, char *temp_path, FilePathInfo **path_info);


// Node structure
void free_path_info_nodes(FilePathInfo **head);
void add_path_info_node(FilePathInfo **head, FilePathInfo *node);
FilePathInfo* create_new_path_info_node(char* absolute_path, char* relative_folder, char* file_name);