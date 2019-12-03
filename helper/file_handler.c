#include "file_handler.h"

void folder_structure(char *root_path, FILE **fo){
    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type
    DIR *dr = opendir(root_path);

    if (dr == NULL) {
        // printf("Could not open current directory");
        return;
    }

    char buffer[256];

    while ((de = readdir(dr)) != NULL) {
        bzero(buffer, sizeof(buffer));
        if (strcmp(root_path, ".") == 0)
            sprintf(buffer, "%s", de->d_name);
        else
            sprintf(buffer, "%s%s", root_path, de->d_name);
        if (isDirectory(buffer) && isValidDirectory(buffer)){
            sprintf(buffer, "%s/", buffer);
            folder_structure(buffer, &(*fo));
        }
        if(!isDirectory(buffer)){
            fprintf(*fo, "%s\n", buffer);
        }
    }

    closedir(dr);
    return;
}

void get_folder_structure(char *root_path, char *temp_path, FilePathInfo **path_info) {
    struct dirent *de;  // Pointer for directory entry
    int i = 0, j, k;

    // opendir() returns a pointer of DIR type
    DIR *dr = opendir(temp_path);

    if (dr == NULL) {
        // printf("Could not open current directory");
        return;
    }

    char buffer[256];
    char absolute_path[512];
    char relative_folder[256];
    char file_name[128];

    while ((de = readdir(dr)) != NULL) {
        bzero(buffer, sizeof(buffer));
        if (strcmp(temp_path, ".") == 0)
            sprintf(buffer, "%s", de->d_name);
        else
            sprintf(buffer, "%s/%s", temp_path, de->d_name);
        if (isDirectory(buffer) && isValidDirectory(buffer)){
            sprintf(buffer, "%s", buffer);
            get_folder_structure(root_path, buffer, &(*path_info));
        }
        if(!isDirectory(buffer)){
            strcpy(absolute_path, buffer);
            i = strlen(absolute_path);
            j = 0;
            k = 0;
  
            while (absolute_path[i] != '/') i--;
            k = i + 1;
            
            while (absolute_path[k] != '\0') {
                file_name[j++] = absolute_path[k++];
            }
            file_name[j] = '\0';
            j = 0;

            for (k = strlen(root_path); k < i; k++) {
                relative_folder[j++] = absolute_path[k];
            }
            relative_folder[j] = '\0';

            add_path_info_node(&(*path_info), create_new_path_info_node(absolute_path, relative_folder, file_name));
        }
    }

    closedir(dr);
    return;
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int isValidDirectory(const char *path) {
    if(strcmp(path, TH_FOLDER) == 0)
        return 0;
    int i = strlen(path) - 1;
    int valid_dir_flag = 0;

    while (path[i] != '/' && i >= 0){
        if (isalpha(path[i]) || isdigit(path[i]) || path[i] == '_')
            valid_dir_flag = 1;
        i--;
    }
    if (valid_dir_flag == 1)
        return 1;
    else 
        return 0;
}

void create_path(char* absolute_path) {
  char cmd[MAXLEN];
  sprintf(cmd, "mkdir -p %s", absolute_path);
  system(cmd);
}

void getUserInfo(char *filename, char *username, char *email) {
    FILE *fi;
    if((fi = fopen(filename, "r")) == NULL) {
        printf("Unable to open file %s\n", filename);
        return;
    }

    // Array of string to get info
    char strArr[10][MAXLEN];
    char str[MAXLEN];

    while(fgets(str, MAXLEN, fi) != NULL) {
        strTokenize(str, strArr, DELIM);
        if (strcmp("username:", strArr[0]) == 0) {
            strcpy(username, strArr[1]);
            username[strlen(username) - 1] = '\0';
        } else if (strcmp("email:", strArr[0]) == 0) {
            strcpy(email, strArr[1]);
            email[strlen(email) - 1] = '\0';
        }
    }

    return;
}

void strTokenize(char *string, char strArr[10][MAXLEN], char *delim) {
    int i = 0;
    char str[MAXLEN];
    char *token;

    token = strtok(string, delim);

    while( token != NULL ) {
        strcpy(strArr[i++], token);
        token = strtok(NULL, delim);
   }

   return;
}

FilePathInfo* create_new_path_info_node(char* absolute_path, char* relative_folder, char* file_name){
    FilePathInfo *newNode = (FilePathInfo*)malloc(sizeof(FilePathInfo));
    strcpy(newNode->absolute_path, absolute_path);
    strcpy(newNode->relative_folder, relative_folder);
    strcpy(newNode->file_name, file_name);
    
    newNode->next = NULL;
    return newNode;
}

void add_path_info_node(FilePathInfo **head, FilePathInfo *node){
    if(*head == NULL){
        *head = node;
        return;
    }

    FilePathInfo *current = *head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = node;
    return;
}

void free_path_info_nodes(FilePathInfo **head){
    FilePathInfo *current = *head;
    FilePathInfo *temp;
    while(current != NULL){
        temp = current;
        current = current->next;
        free(temp);        
    }
}