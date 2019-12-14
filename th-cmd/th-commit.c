#include "../helper/file_handler.h"
#include <time.h>


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
    char username[MAXLEN/4];
    char email[MAXLEN/4];
    char message[MAXLEN/4];
    char commit_name[MAXLEN/4];
    char commit[MAXLEN];
    char first_line[MAXLEN/4];
    char temp[MAXLEN/4];
    int i, number_of_commits;

    char create_folder_cmd[MAXLEN];
    char copy_cmd[MAXLEN];
    char commit_cmd[MAXLEN];

    char commit_file[MAXLEN/4];
    char user_file[MAXLEN/4];

    sprintf(commit_file, ".th/%s", COMMIT_FILE);
    sprintf(user_file, ".th/%s", USER_FILE);

    getUserInfo(user_file, username, email);
    sprintf(commit_name, "commit-%d", time(NULL));
    strcpy(message, argv[2]);
    sprintf(commit, "%s %s %s msg: %s\n", commit_name, username, email, message);

    FILE *fc;
    if ((fc = fopen(commit_file, "r+")) == NULL){
        printf("Unable to open file %s\n", commit_file);
        exit(1);
    }

    // Move the cusor to begining of the file
    rewind(fc);
    if (fgets(first_line, MAXLEN, fc) != NULL)
        sscanf(first_line, "%d %s", &number_of_commits, temp);
    // Increase number of commits
    number_of_commits++;
    // Create folder cmd
    sprintf(create_folder_cmd, "mkdir -p .th/commits/%s", commit_name);
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

    return;
}