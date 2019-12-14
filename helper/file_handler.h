/*******************
* FILE HANDLER
********************
*
* Validate folder
* Create folders, files
* Get path, insert content to files and more
*
********************/

#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* Single link list for handling file path */
typedef struct PathInfo {
    char absolute_path[512];
    char relative_folder[256];
    char file_name[128];
    struct PathInfo *next;
} FilePathInfo;

/* Single link list for storing commits name */
typedef struct commit {
    char full_commit[512];
    char commit_name[128];
    struct commit *next;
} Commit;

/* folder and file name */
#define TH_FOLDER ".th"
#define COMMIT_FOLDER "commits"
#define COMMIT_FILE "commit_log"
#define USER_FILE "user_info"

#define MAXLEN 1024
#define DELIM " "

/* < Check If Input Path Is A Directory >
* @path (input): path to directory
*/
int isDirectory(const char *path);

/* < Check if the directory is valid (../ and ./ is invalid) >
*/
int isValidDirectory(const char *path);

/* < Tokenize A String Into Tokens >
* @string (input): string to be tokenized
* @strArray (return value): store tokenized string
* @delim (input): delimiter
*/
void strTokenize(char *string, char strArr[10][MAXLEN], char *delim);

/* < Get User Info >
* @filename (input): file stores user information
* @username (return value): user's name
* @email (return value): user's email
*/
void getUserInfo(char *filename, char *username, char *email);

/* < Create A Folder With Absolute Path >
* @absolute_path (input): path will be created
*/
void create_path(char* absolute_path);

/* < Get Folder Structure >
* @root_path (input): absolute path
* @temp_path (input): tempory path
* @path_info (return value): Contain information about folder structure
*/
void get_folder_structure(char *root_path, char *temp_path, FilePathInfo **path_info);

/* < Append File Content >
* @absolute_file_path (input): absolute path to file
* @absolute_folder_path (input): absolute path to folder contains file
* @content (input): content that will be written into file
*/
void append_file_content(char* absolute_file_path, char* absolute_folder_path, char *content);

/* < Get current location path >
* @path (return value): current location path
*/
void get_pwd(char *path);

/* < Get Commit History >
* @absolute_file_path (input): absolute path to commit file
* @head (return value): link list contains all commits
*/
void get_commits_history(char* absolute_folder_path, Commit** head);

/* < Get Repository Name >
* @repo_name (return value): name of repository that you are current in
*/
int get_repo_name(char *repo_name);

/* Path Information Node  */
void free_path_info_nodes(FilePathInfo **head);
void add_path_info_node(FilePathInfo **head, FilePathInfo *node);
FilePathInfo* create_new_path_info_node(char* absolute_path, char* relative_folder, char* file_name);

/* Commit Node */
void free_commit_nodes(Commit **head);
void add_commit_node(Commit **head, Commit *node);
Commit* create_new_commit_node(char* full_commit, char* commit_name);
Commit* remove_data_node(Commit *head, char *commit_name);