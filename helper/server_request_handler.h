#include "../helper/message_handler.h"
#include "../helper/network_config.h"
#include "../helper/file_handler.h"

int login_handler(int sockfd, char* buffer, char* email, char* password);
void signup_handler(int sockfd, char* buffer);
void create_repo_handler(int sockfd, char* buffer, char *username, char*email);
void list_repo_handler(int sockfd, char* buffer, char *username);
int logout_handler(int sockfd, char* buffer);
void clone_repo_handler(int sockfd, char* buffer, char *username);

void get_main_folder_location(char *path);