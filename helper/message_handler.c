#include "message_handler.h"

/**
* LOGIN HANDLE FUNCTION
**/
void login_msg_request_encoder(char* encode_msg, char* email, char* password){
    int i = 0, t = 0;
    bzero(encode_msg, MSG_MAX_LEN);

    while(i < MSG_FLAG_LEN) {
        encode_msg[i++] = LOGIN_FLAG[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(email)){
        encode_msg[i++] = email[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(password)){
        encode_msg[i++] = password[t++];
    }
    encode_msg[i++] = '\0';

    return;
}

void login_msg_request_decoder(char* encoded_msg, char* email, char *password){
    int i = MSG_FLAG_LEN + 1, t = 0;
    while(encoded_msg[i] != '\n'){
        email[t++] = encoded_msg[i++];
    }
    email[t] = '\0';
    i++;
    t = 0;
    while(encoded_msg[i] != '\0'){
        password[t++] = encoded_msg[i++];
    }
    password[t] = '\0';
    return;  
}

void login_msg_response_encoder(char* encode_msg, char* username){
    int i = 0, t = 0;
    bzero(encode_msg, MSG_MAX_LEN);

    while(i < RESPONSE_STATUS_LEN) {
        encode_msg[i++] = RESPONSE_OK[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(username)){
        encode_msg[i++] = username[t++];
    }
    encode_msg[i++] = '\0';
    return;
}

void login_msg_response_decoder(char* encoded_msg, char *response_status, char* username){
    int i = 0, t = 0;
    while(i < RESPONSE_STATUS_LEN){
        response_status[t++] = encoded_msg[i++];
    }
    response_status[t] = '\0';
    i++;
    t = 0;
    while(encoded_msg[i] != '\0'){
        username[t++] = encoded_msg[i++];
    }
    username[t] = '\0';
    return;  
}

/**
* SIGNUP HANDLE FUNCTION
**/
void signup_msg_request_encoder(char* encode_msg, char* username, char* email, char* password){
    int i = 0, t = 0;
    bzero(encode_msg, MSG_MAX_LEN);

    while(i < MSG_FLAG_LEN) {
        encode_msg[i++] = SIGNUP_FLAG[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(username)){
        encode_msg[i++] = username[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(email)){
        encode_msg[i++] = email[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(password)){
        encode_msg[i++] = password[t++];
    }
    encode_msg[i++] = '\0';

    return;
}

void signup_msg_request_decoder(char* encoded_msg, char* username, char* email, char *password){
    int i = MSG_FLAG_LEN + 1, t = 0;

    while(encoded_msg[i] != '\n'){
        username[t++] = encoded_msg[i++];
    }
    username[t] = '\0';
    i++;
    t = 0;

    while(encoded_msg[i] != '\n'){
        email[t++] = encoded_msg[i++];
    }
    email[t] = '\0';
    i++;
    t = 0;

    while(encoded_msg[i] != '\0'){
        password[t++] = encoded_msg[i++];
    }
    password[t] = '\0';
    return;  
}

void signup_msg_response_encoder(char* encode_msg, char* username){
    int i = 0, t = 0;
    bzero(encode_msg, MSG_MAX_LEN);

    while(i < RESPONSE_STATUS_LEN) {
        encode_msg[i++] = RESPONSE_OK[t++];
    }
    encode_msg[i++] = '\n';
    t = 0;

    while(t < strlen(username)){
        encode_msg[i++] = username[t++];
    }
    encode_msg[i++] = '\0';
    return;
}

void signup_msg_response_decoder(char* encoded_msg, char *response_status, char* username){
    int i = 0, t = 0;
    while(i < RESPONSE_STATUS_LEN){
        response_status[t++] = encoded_msg[i++];
    }
    response_status[t] = '\0';
    i++;
    t = 0;

    while(encoded_msg[i] != '\0'){
        username[t++] = encoded_msg[i++];
    }
    username[t] = '\0';
    return;  
}