#include "file_handler.h"

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
            bzero(absolute_path, sizeof(absolute_path));
            bzero(relative_folder, sizeof(relative_folder));
            bzero(file_name, sizeof(file_name));
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

void append_file_content(char* absolute_file_path, char* absolute_folder_path, char *content) {
    FILE *fi;
    char create_folder_cmd[256];
    printf("\n-------------------------------\n");
    sprintf(create_folder_cmd, "mkdir -p %s", absolute_folder_path);
    system( create_folder_cmd );
    printf("file_path: %s\n", absolute_file_path);

    if((fi = fopen(absolute_file_path, "a+")) == NULL) {
        printf("Failed\n");
        return;
    }
    printf("Ok\n");
    fprintf(fi, "%s", content);
    fclose(fi);
    printf("\n-------------------------------\n");

    return;
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

void get_pwd(char *path){
  FILE *fp;
  char temp[1024];
  int i;
  bzero(path, sizeof(path));

  /* Open the command for reading. */
  fp = popen("pwd", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while(fgets(temp, sizeof(path), fp) != NULL) {
      strcat(path, temp);
  }
  path[strlen(path)-1] = '\0';
  /* close */
  pclose(fp);

}

void get_commits_history(char* absolute_folder_path, Commit** head) {
    char commit_file_path[MAXLEN];
    char line[MAXLEN], full_commit[MAXLEN/2], commit_name[MAXLEN/2];
    char strArr[10][MAXLEN];
    Commit *temp;
    FILE *fi;
    sprintf(commit_file_path, "%s/.th/%s", absolute_folder_path, COMMIT_FILE);
    if((fi = fopen(commit_file_path, "r")) == NULL){
        printf("Unable to open file: %s\n", COMMIT_FILE);
        return;
    }
    // Skip number of commits line
    if(fgets(line, MAXLEN, fi) == NULL){
        printf("Ops, something went wrong!\n");
        return;
    }
    while(fgets(line, MAXLEN, fi) != NULL) {
        line[strlen(line)-1] = '\0';
        strcpy(full_commit, line);
        strTokenize(line, strArr, " ");
        strcpy(commit_name, strArr[0]);
        add_commit_node(&(*head), create_new_commit_node(full_commit, commit_name));
    }

    fclose(fi);
    return;
}

int get_repo_name(char *repo_name) {
    FILE *fi;
    char line[MAXLEN], strArr[10][MAXLEN];
    char user_file_path[MAXLEN];
    char pwd[MAXLEN];
    
    get_pwd(pwd);
    sprintf(user_file_path, "%s/.th/%s", pwd, USER_FILE);
    if((fi = fopen(user_file_path, "r")) == NULL){
        return 0;
    }

    while(fgets(line, MAXLEN, fi) != NULL) {
        strTokenize(line, strArr, " ");
        if(strcmp(strArr[0], "repository:") == 0){
            strcpy(repo_name, strArr[1]);
            return 1;
        }
    }

    fclose(fi);
    return 1;
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

void addCommitToLog(char *commit_file_path, char *commit_msg) {
    FILE *fc;
    char first_line[MAXLEN], temp[MAXLEN/4];
    int number_of_commits;
    if ((fc = fopen(commit_file_path, "r+")) == NULL){
        printf("Unable to open file %s\n", commit_file_path);
        return;
    }

    // Move the cusor to begining of the file
    rewind(fc);
    if (fgets(first_line, MAXLEN, fc) != NULL)
        sscanf(first_line, "%d %s", &number_of_commits, temp);
    // Increase number of commits
    number_of_commits++;
    
    // Update commit_log
    fseek(fc, 0, SEEK_END);
    fprintf(fc, "%s\n", commit_msg);
    rewind(fc);
    fprintf(fc, "%d commits", number_of_commits);

    fclose(fc);
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

Commit* create_new_commit_node(char* full_commit, char* commit_name){
    Commit *newNode = (Commit*)malloc(sizeof(Commit));

    strcpy(newNode->full_commit, full_commit);
    strcpy(newNode->commit_name, commit_name);
    
    newNode->next = NULL;
    return newNode;
}

void add_commit_node(Commit **head, Commit *node){
    if(*head == NULL){
        *head = node;
        return;
    }

    Commit *current = *head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = node;
    return;
}

Commit* remove_data_node(Commit *head, char *commit_name) {
    Commit *temp, *current;
    if (head == NULL) {
        return NULL;
    }
    
    current = head;
    if (strcmp(current->commit_name, commit_name) == 0) {
        temp = current;
        head = head->next;
        free(temp);
        return head;
    }
    while (current->next != NULL) {

        if (strcmp(current->next->commit_name, commit_name) == 0) {
            printf("y\n");
            temp = current->next;
            current->next = current->next->next;
            free(temp);
        }
        current = current->next;
    }
    return head;
}

void free_commit_nodes(Commit **head){
    Commit *current = *head;
    Commit *temp;
    while(current != NULL){
        temp = current;
        current = current->next;
        free(temp);        
    }
}