#include "../helper/message_handler.h"
#include "../helper/network_config.h"

int login_handler(int sockfd, char* buffer, char* email, char* password);
void signup_handler(int sockfd, char* buffer, char* username, char* email, char* password);