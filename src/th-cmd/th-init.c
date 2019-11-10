#include <stdlib.h>
#include "src/fileHandler.h"

#define MAXLEN 1024
#define LOG_FILE ".th/structure_log"
#define COMMIT_FILE ".th/commit_log"
#define USER_FILE ".th/user_info"

void setUserInfo(FILE **fi);

void main () {
    
    char init_cmd[MAXLEN];

    char *create_th_folder = "mkdir -p .th/commits";
    char *create_th_commit_log = "touch .th/commit_log";
    char *create_th_structure_log = "touch .th/structure_log";

    // Build up creating folder structure cmd 
    strcat(init_cmd, create_th_folder);
    strcat(init_cmd, " && ");
    strcat(init_cmd, create_th_commit_log);
    strcat(init_cmd, " && ");
    strcat(init_cmd, create_th_structure_log);

    system(init_cmd);

    FILE *fo, *fc, *fu;
    if((fo = fopen(LOG_FILE, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", LOG_FILE);
        exit(1);
    }

    if((fc = fopen(COMMIT_FILE, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", COMMIT_FILE);
        exit(1);
    }

    if((fu = fopen(USER_FILE, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", USER_FILE);
        exit(1);
    }
    // Also write current repo structure to structure_log 
    folder_structure(".", &fo);
    // Also init commit_log
    fprintf(fc, "0 commits\n");
    // Also create a user file information
    setUserInfo(&fu);

    fclose(fo);
    fclose(fc);
    fclose(fu);

    system( "echo Initialize th version control at && pwd" );
    

}


void setUserInfo(FILE **fi) {
    char username[MAXLEN], email[MAXLEN];
    int i, j;
    
    do {
        i = 0;
        bzero(username, sizeof(username));  
        printf("Input your username: ");
        while ((username[i++] = getchar()) != '\n');
        username[i-1] = '\0';
        if(username[0] == '\0')
            printf("User name can't be blank!");
    }while (username[0] == '\0');

    do {
        j = 0;
        bzero(email, sizeof(email));  
        printf("Input your email: ");
        while ((email[j++] = getchar()) != '\n');
        email[j-1] = '\0';
        if(email[0] == '\0')
            printf("email can't be blank!");
    }while (email[0] == '\0');

    fprintf(*fi, "username: %s\n", username);
    fprintf(*fi, "email: %s\n", email);
    return;

}