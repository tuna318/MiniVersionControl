#include "server_request_handler.h"

int login_handler(int sockfd, char* buffer, char* email, char* password) {
    char send_msg[MSG_MAX_LEN];
    login_msg_request_decoder(buffer, email, password);
    login_msg_response_encoder(send_msg, email);
    write(sockfd, send_msg, strlen(send_msg));
    return 1;
}

void signup_handler(int sockfd, char* buffer) {
    char send_msg[MSG_MAX_LEN];
    char username[USERNAME_LEN], email[EMAIL_LEN], password[PASSWORD_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char excution_cmd[MSG_MAX_LEN];

    signup_msg_request_decoder(buffer, username, email, password);
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
    sprintf(th_init_cmd, "th-init %s %s", username, email);
    strcat(excution_cmd, naviagte_to_storage_cmd);
    strcat(excution_cmd, " && ");
    strcat(excution_cmd, th_init_cmd);

    printf("cmd: %s\n", excution_cmd);

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
    char send_msg[MSG_MAX_LEN];
    char repo_name[REPONAME_LEN];
    char main_folder_path[MSG_MAX_LEN];
    char repo_absolute_path[MSG_MAX_LEN];

    bzero(username, sizeof(username));
    strcpy(username, "tuna");
    clone_repo_msg_request_decoder(buffer, repo_name);
    get_main_folder_location(main_folder_path);
    sprintf(repo_absolute_path, "%s/%s/%s", main_folder_path, username, repo_name);


    // list_repo_msg_response_encoder(send_msg, list_repo);
    write(sockfd, send_msg, strlen(send_msg));
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