#include "../helper/file_handler.h"


void setUserInfo(FILE **fi, char *username, char *email);

void main (int argc, char *argv[]) {
    
    if (argc != 3) {
        printf("Number of arguments for th-init not match!\n");
        exit(1);
    }

    char init_cmd[MAXLEN];

    char create_th_folder[MAXLEN];
    char create_th_commit_log[MAXLEN];
    char create_th_structure_log[MAXLEN];
    char create_user_log[MAXLEN];
    char log_file[MAXLEN], commit_file[MAXLEN], user_file[MAXLEN];
    char username[MAXLEN], email[MAXLEN];
    strcpy(username, argv[1]);
    strcpy(email, argv[2]);

    sprintf(create_th_folder, "mkdir -p %s/%s", TH_FOLDER, COMMIT_FOLDER);
    sprintf(create_th_commit_log, "touch %s/%s", TH_FOLDER, COMMIT_FILE);
    sprintf(create_th_structure_log, "touch %s/%s", TH_FOLDER, LOG_FILE);
    sprintf(create_user_log, "touch %s/%s", TH_FOLDER, USER_FILE);
    
    sprintf(log_file, "%s/%s", TH_FOLDER, LOG_FILE);
    sprintf(commit_file, "%s/%s", TH_FOLDER, COMMIT_FILE);
    sprintf(user_file, "%s/%s", TH_FOLDER, USER_FILE);

    // Build up creating folder structure cmd 
    bzero(init_cmd, sizeof(init_cmd));
    strcat(init_cmd, create_th_folder);
    strcat(init_cmd, " && ");
    strcat(init_cmd, create_th_commit_log);
    strcat(init_cmd, " && ");
    strcat(init_cmd, create_th_structure_log);
    strcat(init_cmd, " && ");
    strcat(init_cmd, create_user_log);

    printf("init cmd: %s\n", init_cmd);

    system(init_cmd);

    FILE *fo, *fc, *fu;
    if((fo = fopen(log_file, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", log_file);
        exit(1);
    }

    if((fc = fopen(commit_file, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", commit_file);
        exit(1);
    }

    if((fu = fopen(user_file, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", user_file);
        exit(1);
    }
    // Also write current repo structure to structure_log 
    folder_structure(".", &fo);
    // Also init commit_log
    fprintf(fc, "0 commits\n");
    // Also create a user file information
    setUserInfo(&fu, username, email);

    fclose(fo);
    fclose(fc);
    fclose(fu);

    system( "echo Initialize successfully" );
    return;
}

void setUserInfo(FILE **fi, char *username, char *email) {
    fprintf(*fi, "username: %s\n", username);
    fprintf(*fi, "email: %s\n", email);
    return;

}