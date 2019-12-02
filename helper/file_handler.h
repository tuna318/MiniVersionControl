#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define th_folder ".th"
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
// Send file content message Encoding
void encode_msg(char FLAG, char *username, char *repo, char *commit, char *file_locate, char* data, char encode_msg[]);
// Send file content message Decoding
void decode_msg(char *FLAG, char username[], char repo[], char commit[], char file_locate[], char data[], char encoded_msg[]);


