#include "server_request_handler.h"

int login_handler(int sockfd, char* buffer, char* email, char* password) {
    char send_msg[MSG_MAX_LEN];
    login_msg_request_decoder(buffer, email, password);
    login_msg_response_encoder(send_msg, email);
    write(sockfd, send_msg, strlen(send_msg));
    return 1;
}

void signup_handler(int sockfd, char* buffer, char* username, char* email, char* password) {
    char send_msg[MSG_MAX_LEN];
    signup_msg_request_decoder(buffer, username, email, password);
    signup_msg_response_encoder(send_msg, username);
    write(sockfd, send_msg, strlen(send_msg));
}