#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_LEN 1024

#define MSG_FLAG_LEN 2
#define LOGIN_FLAG "01"
#define SIGNUP_FLAG "02"

#define RESPONSE_STATUS_LEN 3
#define RESPONSE_OK "200"
#define RESPONSE_NOTFOUND "400"
#define RESPONSE_INTERNAL_ERR "500"

#define USERNAME_LEN 32
#define EMAIL_LEN 32
#define PASSWORD_LEN 32

#define CHECK_CONNECTION "CHECK CONNECTION"


/**
* LOGIN HANDLE FUNCTIONS
**/
// Login request message encoder
void login_msg_request_encoder(char* encoded_msg, char* email, char* password);
// Login request message decoder
void login_msg_request_decoder(char* encoded_msg, char* email, char *password);
// Login response message encoder
void login_msg_response_encoder(char* encode_msg, char* username);
// Login response message decoder
void login_msg_response_decoder(char* encoded_msg, char *response_status, char* username);

/**
* SIGNUP HANDLE FUNCTIONS
**/
// Signup request message encoder
void signup_msg_request_encoder(char* encode_msg, char* username, char* email, char* password);
// Signup request message decoder
void signup_msg_request_decoder(char* encoded_msg, char* username, char* email, char *password);
// Signup response message encoder
void signup_msg_response_encoder(char* encode_msg, char* username);
// Signup response message dencoder
void signup_msg_response_decoder(char* encoded_msg, char *response_status, char* username);

