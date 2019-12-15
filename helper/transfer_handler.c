#include "transfer_handler.h"

int transfer_a_commit(int sockfd, char* repo_name, Commit* commit, char* commit_absolute_path){
    FilePathInfo* path_info = NULL, *temp;
    char line[FILE_CONTENT_LEN], send_msg[MSG_MAX_LEN],
         received_msg[MSG_MAX_LEN], res_status[32];

    get_folder_structure(commit_absolute_path,
                         commit_absolute_path,
                         &path_info);
    temp = path_info;
    while (temp != NULL) {
        FILE *fi;
        if((fi = fopen(temp->absolute_path, "r")) == NULL)
            continue;

        while( fgets(line, FILE_CONTENT_LEN, fi) != NULL ) {
            // sleep(1);
            // Encode and send msg
            bzero(send_msg, sizeof(send_msg));
            send_commits_request_encoder(send_msg,
                                         PUSH_FLAG,
                                         SENDING,
                                         repo_name, 
                                         commit->commit_name, 
                                         temp->relative_folder, 
                                         temp->file_name,
                                         line);
            
            write(sockfd, send_msg, sizeof(send_msg));
            
        }        
        fclose(fi);
        temp = temp->next;
    }

    bzero(send_msg, sizeof(send_msg));
    send_commits_request_encoder(send_msg, PUSH_FLAG, COMPLETED, repo_name, "", "", "", commit->full_commit);
    write(sockfd, send_msg, sizeof(send_msg));
    free_path_info_nodes(&path_info);
    return 1;
}