#include "src/fileHandler.h"
#include <time.h>

#define LOG_FILE ".th/structure_log"
#define COMMIT_FILE ".th/commit_log"
#define USER_FILE ".th/user_info"

void main(int argc, char *argv[]) {
    struct stat s;
    int err = stat(".th", &s);
    if(-1 == err) {
        if(ENOENT == errno) {
            printf("You haven't init th-vs\n");
            exit(1);
        } else {
            perror("stat");
            exit(1);
        }
    }
    if (argc != 3) {
        printf("th-commit format wrong!\n");
        printf("Try: th-commit -m <message>\n");
        exit(1);
    }
    char username[MAXLEN];
    char email[MAXLEN];
    char message[MAXLEN];
    char commit_name[MAXLEN];
    char commit[MAXLEN*4];
    char first_line[MAXLEN];
    char temp[MAXLEN];
    int i, number_of_commits;

    char create_folder_cmd[MAXLEN];
    char copy_cmd[MAXLEN];
    char commit_cmd[MAXLEN];

    getUserInfo(USER_FILE, username, email);
    sprintf(commit_name, "commit-%d", time(NULL));
    strcpy(message, argv[2]);
    sprintf(commit, "%s %s %s msg: %s\n", commit_name, username, email, message);

    FILE *fc, *fo;
    if ((fc = fopen(COMMIT_FILE, "r+")) == NULL){
        printf("Unable to open file %s\n", COMMIT_FILE);
        exit(1);
    }
    if((fo = fopen(LOG_FILE, "w")) == NULL){
        printf("INFO: Unable to open file %s\n", LOG_FILE);
        exit(1);
    }

    // Also write current repo structure to structure_log 
    folder_structure(".", &fo);
    // Move the cusor to begining of the file
    rewind(fc);
    if (fgets(first_line, MAXLEN, fc) != NULL)
        sscanf(first_line, "%d %s", &number_of_commits, temp);
    // Increase number of commits
    number_of_commits++;
    // Create folder cmd
    sprintf(create_folder_cmd, "mkdir .th/commits/%s", commit_name);
    // Create copy cmd
    sprintf(copy_cmd, "rsync -av --progress . .th/commits/%s --exclude .th", commit_name);
    
    // Create_full_cmd
    strcat(commit_cmd, create_folder_cmd);
    strcat(commit_cmd, " && ");
    strcat(commit_cmd, copy_cmd);

    // Execute command
    system(commit_cmd);

    // Update commit_log
    fseek(fc, 0, SEEK_END);
    fprintf(fc, "%s", commit);
    rewind(fc);
    fprintf(fc, "%d commits", number_of_commits);
    
    fclose(fc);
    fclose(fo);

    return;
}