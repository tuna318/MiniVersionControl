
/*******************
* MESSAGE HANDLER
********************
*
* Encode/decode request/response and more
*
********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_LEN 2048
#define FILE_CONTENT_LEN 1024

/* Request flags */
#define MSG_FLAG_LEN 2
#define AUTH_FLAG "01"
#define SIGNUP_FLAG "02"
#define CREATE_REPO_FLAG "03"
#define LIST_REPO_FLAG "04"
#define LOGOUT_FLAG "05"
#define CLONE_REPO_FLAG "06"
#define CHECK_COMMITS_FLAG "07"
#define PUSH_FLAG "08"
#define PULL_FLAG "09"

/* Response flags */
#define RESPONSE_STATUS_LEN 3
#define RESPONSE_OK "200"
#define RESPONSE_NOTFOUND "400"
#define RESPONSE_INTERNAL_ERR "500"

#define SENDING "SENDING"
#define COMPLETED "COMPLETED"
#define FAILED "FAILED"
#define PULL_COMPLETED "PULL_COMPLETED"

/* Length of some variables */
#define USERNAME_LEN 32
#define EMAIL_LEN 32
#define REPONAME_LEN 32
#define COMMIT_LEN 32
#define PASSWORD_LEN 32
#define FILE_LOCATION_LEN 128
#define FILE_NAME_LEN 128

#define CHECK_CONNECTION "CHECK CONNECTION"


/**
* AUTH HANDLE FUNCTIONS
**/
// auth request message encoder | Input: @email, @password | Return value: @encode_msg
void auth_msg_request_encoder(char* encode_msg, char* email, char* password);
// Auth request message decoder | Input: @encode_msg | Return value: @email, @password
void auth_msg_request_decoder(char* encoded_msg, char* email, char *password);
// Auth response message encoder | Input: @username | Return value: @encode_msg
void auth_msg_response_encoder(char* encode_msg, char *response_status, char* username);
// Auth response message decoder | Input: @encoded | Return value: @response_status, @username
void auth_msg_response_decoder(char* encoded_msg, char *response_status, char* username);

/**
* SIGNUP HANDLE FUNCTIONS
**/
// Signup request message encoder | Input: @username, @email, @password | Return value: @encode_msg
void signup_msg_request_encoder(char* encode_msg, char* username, char* email, char* password);
// Signup request message decoder | Input: @encoded_msg | Return value: @username, @email, @password
void signup_msg_request_decoder(char* encoded_msg, char* username, char* email, char *password);
// Signup response message encoder | Input: @username, @response_status | Return value: @encode_msg
void signup_msg_response_encoder(char* encode_msg, char *response_status, char* username);
// Signup response message dencoder | Input: @encoded_msg| Return value: @response_status, @username
void signup_msg_response_decoder(char* encoded_msg, char *response_status, char* username);

/**
* CREATE REPOSITORY HANDLE FUNCTIONS
**/
// Create repository request message encoder | Input: @repo_name | Return value: @encode_msg
void create_repo_msg_request_encoder(char* encode_msg, char* repo_name);
// Create repository request message decoder | Input: @encoded_msg | Return value: @repo_name
void create_repo_msg_request_decoder(char* encoded_msg, char* repo_name);
// Create repository response message encoder | Input: @repo_name | Return value: @encode_msg
void create_repo_msg_response_encoder(char* encode_msg, char *response_status, char* repo_name);
// Create repository response message dencoder | Input: @encoded_msg | Return value: @response_status, @repo_name
void create_repo_msg_response_decoder(char* encoded_msg, char *response_status, char *repo_name);

/**
* LIST REPOSITORIES HANDLE FUNCTIONS
**/
// List repositories request message encoder | Input: | Return value: @encode_msg
void list_repo_msg_request_encoder(char* encode_msg);
// List repositories request message decoder | Input: @encoded_msg | Return value: 
void list_repo_msg_request_decoder(char* encoded_msg);
// List repositories response message encoder | Input: @list_repo | Return value: @encode_msg
void list_repo_msg_response_encoder(char* encode_msg, char *response_status, char* list_repo);
// List repositories response message dencoder | Input: @encoded_msg | Return value: @response_status, @list_repo
void list_repo_msg_response_decoder(char* encoded_msg, char *response_status, char *list_repo);

/**
* LOGOUT HANDLE FUNCTIONS
**/
// Logout request message encoder | Input: | Return value: @encode_msg
void logout_msg_request_encoder(char* encode_msg);
// Logout request message decoder | Input: @encoded_msg | Return value: 
void logout_msg_request_decoder(char* encoded_msg);
// Logout response message encoder | Input: | Return value: @encode_msg
void logout_msg_response_encoder(char* encode_msg);
// Logout response message descoder | Input: @encoded_msg | Return value: @response_status
void logout_msg_response_decoder(char* encoded_msg, char *response_status);

/**
* SEND STATUS
**/
void send_status_encoder(char* encode_msg, char* status);
void send_status_decoder(char* encoded_msg, char *status);

/**
* CLONE A REPOSITORY HANDLE FUNCTIONS
**/
// Clone a repository request message encoder | Input: @repo_name | Return value: @encode_msg
void clone_repo_msg_request_encoder(char* encode_msg, char* repo_name);
// Clone a repository request message decoder | Input: @encoded_msg | Return value: @repo_name 
void clone_repo_msg_request_decoder(char* encoded_msg, char* repo_name);
// Clone a repository response message encoder | Input: @response_status, @repo_name, @file_location, @file_name, @content | Return value: @encode_msg
void clone_repo_msg_response_encoder(char* encode_msg, char* response_status, char* repo_name, char* file_location, char *file_name, char *content);
// Clone a repository response message decoder | Input: @encoded_msg | Return value: @response_status, @repo_name, @file_location, @file_name, @content
void clone_repo_msg_response_decoder(char* encoded_msg, char *response_status, char* repo_name, char* file_location, char *file_name, char *content);

/**
* CHECK NEW COMMITS HANDLE FUNCTION
**/
// Compare commits between local and server request message encoder | Input: @repo_name | Return value: @encode_msg
void get_server_commits_msg_request_encoder(char* encode_msg, char* repo_name);
// Compare commits between local and server request message decoder | Input: @encoded_msg | Return value: @repo_name
void get_server_commits_msg_request_decoder(char* encoded_msg, char* repo_name);
// Compare commits between local and server response message encoder | Input: @response_status, @commit | Return value: @encode_msg
void get_server_commits_msg_response_encoder(char* encode_msg, char* response_status, char* commit);
// Compare commits between local and server response message decoder | Input: @encoded_msg | Return value: @response_status, @commit
void get_server_commits_msg_response_decoder(char* encoded_msg, char *response_status, char *commit);


/**
* DEFINE PULLIN REPOSITORY HANDLE FUNCTION
**/
// Define which repository will be pulled request message encoder
void define_pull_repo_msg_request_encoder(char *encode_msg, char *msg_flag, char *repo_name);
// Define which repository will be pulled request message encoder
void define_pull_repo_msg_request_decoder(char *encoded_msg, char *repo_name);
// Define which repository will be pulled response message encoder
void define_pull_repo_msg_response_encoder(char* encode_msg, char *response_status);
// Define which repository will be pulled response message decoder
void define_pull_repo_msg_response_decoder(char* encoded_msg, char *response_status);


/***
* SEND LOCAL COMMIT LOGS HANDLER
***/
// Compare commits between local and server response message encoder | Input: @request_status, @commit | Return value: @encode_msg
void send_local_commits_msg_request_encoder(char* encode_msg, char* request_status, char* commit);
// Compare commits between local and server response message decoder | Input: @encoded_msg | Return value: @request_status, @commit
void send_local_commits_msg_request_decoder(char* encoded_msg, char* request_status, char* commit);

/**
* SEND COMMITS HANDLER
*/
// Send commits request message encoder | Input: @response_status, @repo_name, @commit, @file_location, @file_name, @content | Return value: @encode_msg
void send_commits_request_encoder(char* encode_msg, char *msg_type, char* request_status, char* repo_name, char* commit, char* file_location, char *file_name, char *content);
// Send commits request message decoder | Input: @encoded_msg | Return value:@response_status, @repo_name, @commit, @file_location, @file_name, @content
void send_commits_request_decoder(char* encoded_msg, char *request_status, char* repo_name, char* commit, char* file_location, char *file_name, char *content);
// Send commits response message encoder | Input: @response_status | Return value: @encode_msg
void send_msg_response_encoder(char* encode_msg, char *response_status);
// Send commits response message descoder | Input: @encoded_msg | Return value: @response_status
void send_msg_response_decoder(char* encoded_msg, char *response_status);