#include "server_request_handler.h"
#include "account_handler.h"

int auth_handler(int sockfd, char* buffer, char* email, char* username) {
    char send_msg[MSG_MAX_LEN];
    char password[PASSWORD_LEN];
    bzero(username, sizeof(username));
    strcpy(username, "tuna");

    auth_msg_request_decoder(buffer, email, password);
    auth_msg_response_encoder(send_msg, email);
    write(sockfd, send_msg, strlen(send_msg));
    return 1;
}

void signup_handler(int sockfd, char* buffer) {
    char send_msg[MSG_MAX_LEN];
    char username[USERNAME_LEN], email[EMAIL_LEN], password[PASSWORD_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char excution_cmd[MSG_MAX_LEN];
    char *nameResponse = malloc(50 * (sizeof(char)));
    *nameResponse = '\0';

    signup_msg_request_decoder(buffer, username, email, password);
    nameResponse = createAccount(email, username, password);
    if(nameResponse != NULL)  strcpy(username,nameResponse);

    get_main_folder_location(main_folder_path);

    // Create excution cmd
    bzero(excution_cmd, sizeof(excution_cmd));
    sprintf(excution_cmd, "cd %s && cd storage && mkdir %s", 
            main_folder_path, username);
    system(excution_cmd);

    signup_msg_response_encoder(send_msg, username);
    write(sockfd, send_msg, strlen(send_msg));
}

void create_repo_handler(int sockfd, char* buffer, char *username, char*email) {
    char send_msg[MSG_MAX_LEN];
    char repo_name[REPONAME_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char excution_cmd[MSG_MAX_LEN];
    char th_init_cmd[MSG_MAX_LEN];
    char naviagte_to_storage_cmd[MSG_MAX_LEN];

    create_repo_msg_request_decoder(buffer, repo_name);
    get_main_folder_location(main_folder_path);
    strcpy(username, "tuna");

    // Create excution cmd
    bzero(excution_cmd, sizeof(excution_cmd));
    sprintf(naviagte_to_storage_cmd, "cd %s && cd storage/%s && mkdir %s && cd %s", 
            main_folder_path, username, repo_name, repo_name);
    sprintf(th_init_cmd, "th-init %s %s %s", username, email, repo_name);
    strcat(excution_cmd, naviagte_to_storage_cmd);
    strcat(excution_cmd, " && ");
    strcat(excution_cmd, th_init_cmd);

    system(excution_cmd);

    create_repo_msg_response_encoder(send_msg, repo_name);
    write(sockfd, send_msg, strlen(send_msg));
}

void list_repo_handler(int sockfd, char* buffer, char *username) {
    char send_msg[MSG_MAX_LEN];
    char list_repo[MSG_MAX_LEN];
    strcpy(list_repo, "hello\ngood_night\n");
    list_repo_msg_request_decoder(buffer);
    list_repo_msg_response_encoder(send_msg, list_repo);
    write(sockfd, send_msg, strlen(send_msg));
}

int logout_handler(int sockfd, char* buffer) {
    char send_msg[MSG_MAX_LEN];
    char list_repo[MSG_MAX_LEN];
    logout_msg_request_decoder(buffer);
    logout_msg_response_encoder(send_msg);
    write(sockfd, send_msg, strlen(send_msg));
    return 0;
}

void clone_repo_handler(int sockfd, char* buffer, char *username) {
    char repo_name[REPONAME_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char repo_absolute_path[MSG_MAX_LEN];

    clone_repo_msg_request_decoder(buffer, repo_name);
    get_main_folder_location(main_folder_path);
    sprintf(repo_absolute_path, "%s/storage/%s/%s", main_folder_path, username, repo_name);

    transfer_a_folder(sockfd, repo_absolute_path, repo_name);

    return;
}

int get_server_commits_handler(int sockfd, char* buffer, char* username){
    char send_msg[MSG_MAX_LEN], received_msg[MSG_MAX_LEN];
    char repo_name[REPONAME_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char repo_absolute_path[MSG_MAX_LEN];
    char absolute_file_path[MSG_MAX_LEN];
    char absolute_folder_path[MSG_MAX_LEN];
    char request_status[32], commit[COMMIT_LEN], file_location[FILE_LOCATION_LEN],
         file_name[FILE_NAME_LEN], content[MSG_MAX_LEN];
    Commit *commit_history, *temp;

    get_server_commits_msg_request_decoder(buffer, repo_name);
    repo_name[strlen(repo_name)-1] = '\0';
    get_main_folder_location(main_folder_path);
    sprintf(repo_absolute_path, "%s/storage/%s/%s", main_folder_path, username, repo_name);
    get_commits_history(repo_absolute_path, &commit_history);

    temp = commit_history;
    while(temp != NULL) {
        bzero(send_msg, MSG_MAX_LEN);
        get_server_commits_msg_response_encoder(send_msg, SENDING, temp->commit_name);

        write(sockfd, send_msg, strlen(send_msg));
        temp = temp->next;
    }
    // Sleep for a while, or client can't catch up
    sleep(1);
    bzero(send_msg, MSG_MAX_LEN);
    get_server_commits_msg_response_encoder(send_msg, COMPLETED, "\0");
    write(sockfd, send_msg, strlen(send_msg));
    free_commit_nodes(&commit_history);

    return 1;
}

int push_commits_handler(int sockfd, char* buffer, char* username) {
    char received_msg[MSG_MAX_LEN], send_msg[MSG_MAX_LEN];
    char main_folder_path[MSG_MAX_LEN/4];
    char absolute_folder_path[MSG_MAX_LEN/4], absolute_file_path[MSG_MAX_LEN/4];
    char request_status[32], repo_name[REPONAME_LEN], commit_name[COMMIT_LEN],
         relative_folder[MSG_MAX_LEN/4], file_name[FILE_NAME_LEN], content[MSG_MAX_LEN/2];


    // Decode the message from client
    send_commits_request_decoder(buffer, request_status, repo_name, 
                                 commit_name, relative_folder, file_name, content);
    
    if(strcmp(request_status, SENDING) == 0 ){
        get_main_folder_location(main_folder_path);
        sprintf(absolute_folder_path, "%s/storage/%s/%s/.th/commits/%s%s", 
                                            main_folder_path, username, repo_name, 
                                            commit_name, relative_folder);
        sprintf(absolute_file_path, "%s/%s", absolute_folder_path, file_name);
        // start writing content to file
        append_file_content(absolute_file_path, absolute_folder_path, content);
        return 1;
    } else if (strcmp(request_status, COMPLETED) == 0) {
        char commit_file_path[MSG_MAX_LEN/4];

        get_main_folder_location(main_folder_path);
        sprintf(commit_file_path, "%s/storage/%s/%s/.th/%s", main_folder_path, username,
                                                             repo_name, COMMIT_FILE);
        addCommitToLog(commit_file_path, content);

        return 1;
    }
    return 0;
}

int pull_commits_handler(int sockfd, char* buffer, char* username) {
    Commit *commit_log, *temp;
    char repo_name[REPONAME_LEN], request_status[32], commit[COMMIT_LEN];
    char send_msg[MSG_MAX_LEN], received_msg[MSG_MAX_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char repo_absolute_path[MSG_MAX_LEN];

    // Decode request msg and get list of commit corresponding to repository
    define_pull_repo_msg_request_decoder(buffer, repo_name);
    get_main_folder_location(main_folder_path);
    sprintf(repo_absolute_path, "%s/storage/%s/%s", main_folder_path, username, repo_name);
    get_commits_history(repo_absolute_path , &commit_log);
    
    // Encode message and notify the client 
    define_pull_repo_msg_response_encoder(send_msg, RESPONSE_OK);
    write(sockfd, send_msg, sizeof(send_msg));

    // Filter commits 
    do {
        bzero(received_msg, sizeof(received_msg));
        if(read(sockfd, received_msg, sizeof(received_msg)) <= 0){
            return 0;   
        }

        send_local_commits_msg_request_decoder(received_msg, request_status, commit);

        if(strcmp(request_status, SENDING) == 0){
            // Remove same commit from list of server-local different commits
            commit_log = remove_data_node(commit_log, commit);
            continue;
        } else if (strcmp(request_status, COMPLETED) == 0){
            break;
        } else {
            printf("Something went wrong!\n");
            return -1;
        } 
    } while(1);

    // Send commits to client 
    char absolute_commit_path[MSG_MAX_LEN];
    temp = commit_log;
    while (temp != NULL) {
        sprintf(absolute_commit_path, "%s/storage/%s/%s/.th/commits/%s", main_folder_path, username, 
                                                                     repo_name, temp->commit_name);
        transfer_a_commit(sockfd, repo_name, temp, absolute_commit_path);
        temp = temp->next;
    }
    bzero(send_msg, sizeof(send_msg));
    send_commits_request_encoder(send_msg, "", PULL_COMPLETED, "", "", "", "", "");
    write(sockfd, send_msg, sizeof(send_msg));
    printf("PULL DONE\n");
    return 1;
}

void get_main_folder_location(char *path){
  FILE *fp;
  char temp[1024];
  int i;
  bzero(path, sizeof(path));

  /* Open the command for reading. */
  fp = popen("which th-server", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while(fgets(temp, sizeof(path), fp) != NULL) {
      strcat(path, temp);
      bzero(temp, sizeof(temp));
  }
  i = strlen(path);
  while(path[i] != '/'){
      path[i--] = '\0';
  }
  i--;
  while(path[i] != '/'){
      path[i--] = '\0';
  }
  path[i] = '\0';

  /* close */
  pclose(fp);

}

int transfer_a_folder(int sockfd, char* folder_absolute_path, char *folder_name){
    FilePathInfo* path_info = NULL, *temp;
    char line[FILE_CONTENT_LEN], send_msg[MSG_MAX_LEN];

    get_folder_structure(folder_absolute_path,
                         folder_absolute_path,
                         &path_info);
    temp = path_info;
    while (temp != NULL) {
        FILE *fi;
        if((fi = fopen(temp->absolute_path, "r")) == NULL)
            continue;

        while( fgets(line, FILE_CONTENT_LEN, fi) != NULL ) {
            bzero(send_msg, sizeof(send_msg));
            clone_repo_msg_response_encoder(send_msg,
                                            SENDING, 
                                            folder_name, 
                                            temp->relative_folder, 
                                            temp->file_name,
                                            line);
            
            write(sockfd, send_msg, sizeof(send_msg));
        }        
        fclose(fi);
        temp = temp->next;
    }

    bzero(send_msg, sizeof(send_msg));
    clone_repo_msg_response_encoder(send_msg, COMPLETED, "", "", "", "");
    write(sockfd, send_msg, sizeof(send_msg));
    free_path_info_nodes(&path_info);
    return 1;
}