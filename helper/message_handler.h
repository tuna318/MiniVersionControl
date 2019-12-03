#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_LEN 1024

#define MSG_FLAG_LEN 2
#define LOGIN_FLAG "01"
#define SIGNUP_FLAG "02"
#define CREATE_REPO_FLAG "03"
#define LIST_REPO_FLAG "04"
#define LOGOUT_FLAG "05"
#define CLONE_REPO_FLAG "06"

#define RESPONSE_STATUS_LEN 3
#define RESPONSE_OK "200"
#define RESPONSE_NOTFOUND "400"
#define RESPONSE_INTERNAL_ERR "500"

#define SENDING "SENDING"
#define COMPLETED "COMPLETED"

#define USERNAME_LEN 32
#define EMAIL_LEN 32
#define REPONAME_LEN 32
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

/**
* CREATE REPOSITORY HANDLE FUNCTIONS
**/
// Create repository request message encoder
void create_repo_msg_request_encoder(char* encode_msg, char* repo_name);
// Create repository request message decoder
void create_repo_msg_request_decoder(char* encoded_msg, char* repo_name);
// Create repository response message encoder
void create_repo_msg_response_encoder(char* encode_msg, char* repo_name);
// Create repository response message dencoder
void create_repo_msg_response_decoder(char* encoded_msg, char *response_status, char *repo_name);

/**
* LIST REPOSITORIES HANDLE FUNCTIONS
**/
// List repositories request message encoder
void list_repo_msg_request_encoder(char* encode_msg);
// List repositories request message decoder
void list_repo_msg_request_decoder(char* encoded_msg);
// List repositories response message encoder
void list_repo_msg_response_encoder(char* encode_msg, char* list_repo);
// List repositories response message dencoder
void list_repo_msg_response_decoder(char* encoded_msg, char *response_status, char *list_repo);

/**
* LOGOUT HANDLE FUNCTIONS
**/
// Logout request message encoder
void logout_msg_request_encoder(char* encode_msg);
// Logout request message decoder
void logout_msg_request_decoder(char* encoded_msg);
// Logout response message encoder
void logout_msg_response_encoder(char* encode_msg);
// Logout response message descoder
void logout_msg_response_decoder(char* encoded_msg, char *response_status);

/**
* SEND STATUS
**/
void send_status_encoder(char* encode_msg, char* status);
void send_status_decoder(char* encoded_msg, char *status);

/**
* CLONE A REPOSITORY HANDLE FUNCTIONS
**/
// Clone a repository request message encoder
void clone_repo_msg_request_encoder(char* encode_msg, char* repo_name);
// Clone a repository request message decoder
void clone_repo_msg_request_decoder(char* encoded_msg, char* repo_name);
// Clone a repository response message encoder
void clone_repo_msg_response_encoder(char* encode_msg, char* repo_name, char* file_location, char *file_name, char *content);
// Clone a repository response message decoder
void clone_repo_msg_response_decoder(char* encoded_msg, char *response_status, char* repo_name, char* file_location, char *file_name, char *content);
