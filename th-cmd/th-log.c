#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h> 


void main() {
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

    system( "cat .th/commit_log");
    return;
}