#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h> 
#include <string.h>

#define MAXLEN 1024


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

    if (argc != 2) {
        printf("th-checkout format wrong!\n");
        printf("Try th-checkout <commit-name>\n");
        exit(1);
    }
    char checkout_cmd[MAXLEN];
    char commit_name[MAXLEN];

    strcpy(commit_name, argv[1]);
    // Create checkout cmd
    sprintf(checkout_cmd, "rm -rf !(.th) && rsync -av --progress .th/commits/%s/*  .", commit_name);
    // Execute checkout_cmd
    system(checkout_cmd);
    return;
}