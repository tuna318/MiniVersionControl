#include "../helper/socket_validate.h" 
#include "../helper/network_config.h"
#include "../helper/message_handler.h"
#include "../helper/file_handler.h"

#define LOGINED 1
#define NOT_LOGIN 0
#define STATUS_OK 1
#define STATUS_FAILED 0
#define SERVER_DISCONNECT -1

void run(int sockfd);                                   // Start the menu
int login(int sockfd, char* username);                  // Login function
void logined_menu(int sockfd, char *username);          // Send text function
int sign_up(int sockfd, char *username);                // Signup handle function
int create_repo(int sockfd, char *repo_name);           // Create repository
int list_repo(int sockfd, char* list_repo);             // Get list repo
int clone_repo(int sockfd, char *repo_name);             // Clone a repo
int logout(int sockfd);                                 // Logout

int main() { 
    
    struct sockaddr_in servaddr, cliaddr;
    char ip_addr[20], port[20];
    int sockfd, connfd;
    
    strcpy(ip_addr, SERVER_IP_ADDRESS);
    sprintf(port, "%d", SERVER_PORT);

    // Validate port
    if(!is_valid_ip(ip_addr) || !is_valid_port(port)){
        printf("ERROR: Invalid format of IP address or port!\n");
        exit(1);
    }
    else {
        printf("Server is running on: %s %d\n",SERVER_IP_ADDRESS, SERVER_PORT);
    }

    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS); 
    servaddr.sin_port = htons(SERVER_PORT); 

    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 

    run(sockfd);
    
    close(sockfd);
} 

/** HELP FUNCTION SECTION **/

void run(int sockfd){
    char selector;
    char username[USERNAME_LEN];
    int login_flag = 0, signup_status = 0;

    do {
        login_flag = NOT_LOGIN;

        printf("Account Manager\n");
        printf("------------------------------------------\n");
        printf("1. Login\n");
        printf("2. Signup\n");
        printf("3. Exit\n");
        printf("Your choice: ");
        scanf("%c%*c", &selector);

        switch(selector){
            case '1':
                /* Login */
                login_flag = login(sockfd, username);
                if (login_flag == LOGINED) {
                    logined_menu(sockfd, username);
                }
                else if (login_flag == NOT_LOGIN) {
                    printf("Email or password wrong!\n");
                }
                else {
                    printf("Internal error\n");
                    close(sockfd);
                    return;
                }
                break;

            case '2': 
                /* Signup */
                signup_status = sign_up(sockfd, username);
                if (signup_status == STATUS_OK) {
                    printf("Account %s registered successfully!\n");
                } else if (signup_status == -2) {
                    printf("Password and Password confirmation not match!\n");
                } else if (signup_status == STATUS_FAILED) {
                    printf("Username or email was used!\nTry again\n");
                } else {
                    printf("Internall error\n");
                    close(sockfd);
                    return;
                }
                break;

            case '3':
                /* Exit */
                printf("Exiting...\n");
                close(sockfd);
                return;

            default:
                if (selector == '\n')
                    break;
                while(getchar() != '\n');
                break;
        }
    } while(1);
    return;
}

void logined_menu(int sockfd, char *username) {
    char selector;
    int login_flag = 0, req_status = 0;
    char repo_name[REPONAME_LEN], list_repositories[MSG_MAX_LEN];

    do {
        printf("Logined as %s\n", username);
        printf("------------------------------------------\n");
        printf("1. Create a repository\n");
        printf("2. List repositories\n");
        printf("3. Clone repository\n");
        printf("4. Logout\n");
        printf("5. Exit\n");
        printf("Your choice: ");
        scanf("%c%*c", &selector);

        switch(selector){
            case '1':
                /* Create a repository */
                req_status = create_repo(sockfd, repo_name);
                if (req_status == STATUS_OK) {
                    printf("create %s successfully\n", repo_name);
                } else if (req_status == STATUS_FAILED) {
                    printf("Failed to create repository!\nTry again\n");
                } else {
                    printf("Internal Error!\n");
                    close(sockfd);
                    return;
                }
                break;

            case '2':
                /* Show list repositories of current user */
                req_status = list_repo(sockfd, list_repositories);
                if (req_status == STATUS_OK) {
                    printf("List repositories:\n %s\n", list_repositories);
                } else if (req_status == STATUS_FAILED) {
                    printf("Failed to get list repositories!\nTry again\n");
                } else {
                    printf("Internal Error!\n");
                    close(sockfd);
                    return;
                }
                break;

            case '3':
                /* Clone a repository from server to client */
                req_status = clone_repo(sockfd, repo_name);
                if (req_status == STATUS_OK) {
                    printf("Clone %s successfully!\n", repo_name);
                } else if (req_status == STATUS_FAILED) {
                    printf("Failed to clone repository!\nTry again\n");
                } else {
                    printf("Internal Error!\n");
                    close(sockfd);
                    return;
                }
                break;

            case '4':
                /* Logout */
                req_status = logout(sockfd);
                if (req_status == STATUS_OK) {
                    printf("Logout successfully!\n");
                    return;
                } else if (req_status == STATUS_FAILED) {
                    printf("Uhm.., something went wrong!\nTry again\n");
                } else {
                    printf("Internal Error!\n");
                    close(sockfd);
                    return;
                }
            case '5':
                /* Exit program */
                close(sockfd);
                exit(0);

            default:
                if (selector == '\n')
                    break;
                while(getchar() != '\n');
                break;
        }
    } while(1);
    return;
}

int login(int sockfd, char* username){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char email[EMAIL_LEN], password[PASSWORD_LEN], 
         res_username[USERNAME_LEN], res_status[RESPONSE_STATUS_LEN+1];
    int i, j;

    // Authentication with email and password
    do {
        i = 0;
        bzero(email, sizeof(email));  
        printf("Input your email: ");
        while ((email[i++] = getchar()) != '\n');
        email[i-1] = '\0';
        if(email[0] == '\0')
            printf("Email can't be blank!");
    }while (email[0] == '\0');

    do {
        j = 0;
        bzero(password, sizeof(password));  
        printf("Input your password: ");
        while ((password[j++] = getchar()) != '\n');
        password[j-1] = '\0';
        if(password[0] == '\0')
            printf("Password can't be blank!");
    }while (password[0] == '\0');

    // Encode and send authentication request to server
    auth_msg_request_encoder(send_msg, email, password);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);

    // Check connection between client and server
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    // Decode response message and check response status
    auth_msg_response_decoder(receive_msg, res_status, res_username);
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(username, USERNAME_LEN);
        strcpy(username, res_username);
        return STATUS_OK;
    } 

    return STATUS_FAILED;
}

int sign_up(int sockfd, char *username){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char email[EMAIL_LEN], password[PASSWORD_LEN], password_confirmation[PASSWORD_LEN],
         res_username[USERNAME_LEN], res_status[RESPONSE_STATUS_LEN+1];
    int i;

    // Input the neccessary information username/email/password/password-vertification
    do {
        i = 0;
        bzero(username, sizeof(username));  
        printf("Input your username: ");
        while ((username[i++] = getchar()) != '\n');
        username[i-1] = '\0';
        if(username[0] == '\0')
            printf("Username can't be blank!");
    }while (username[0] == '\0');

    do {
        i = 0;
        bzero(email, sizeof(email));  
        printf("Input your email: ");
        while ((email[i++] = getchar()) != '\n');
        email[i-1] = '\0';
        if(email[0] == '\0')
            printf("Email can't be blank!");
    }while (email[0] == '\0');


    do {
        i = 0;
        bzero(password, sizeof(password));  
        printf("Input your password: ");
        while ((password[i++] = getchar()) != '\n');
        password[i-1] = '\0';
        if(password[0] == '\0')
            printf("Password can't be blank!");
    }while (password[0] == '\0');
    
    do {
        i = 0;
        bzero(password_confirmation, sizeof(password_confirmation));  
        printf("Input your password_confirmation: ");
        while ((password_confirmation[i++] = getchar()) != '\n');
        password_confirmation[i-1] = '\0';
        if(password_confirmation[0] == '\0')
            printf("Password_confirmation can't be blank!");
    }while (password_confirmation[0] == '\0');

    if (strcmp(password_confirmation, password) != 0){
        return -2;
    }
    
    // Encode and send create account request
    signup_msg_request_encoder(send_msg, username, email, password);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);

    // Check connection between client and server
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    // Decode response message and check response status
    signup_msg_response_decoder(receive_msg, res_status, res_username);
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(username, USERNAME_LEN);
        strcpy(username, res_username);
        return STATUS_OK;
    }

    return STATUS_FAILED;
}

int create_repo(int sockfd, char *repo_name){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char res_repo_name[REPONAME_LEN], res_status[RESPONSE_STATUS_LEN+1];
    int i;

    // Name of repository want to create
    do {
        i = 0;
        bzero(repo_name, sizeof(repo_name));  
        printf("Input your repository name: ");
        while ((repo_name[i++] = getchar()) != '\n');
        repo_name[i-1] = '\0';
        if(repo_name[0] == '\0')
            printf("Repository name can't be blank!");
    }while (repo_name[0] == '\0');

    // Encode message and send creating repository request
    create_repo_msg_request_encoder(send_msg, repo_name);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);

    // Check connection between client and server
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    // Decode message and check response status
    create_repo_msg_response_decoder(receive_msg, res_status, res_repo_name);
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(repo_name, REPONAME_LEN);
        strcpy(repo_name, res_repo_name);
        return STATUS_OK;
    }

    return STATUS_FAILED;
}

int list_repo(int sockfd, char* list_repositories){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char res_list_repo[MSG_MAX_LEN], res_status[RESPONSE_STATUS_LEN+1];

    // Encode and send list repositories request
    list_repo_msg_request_encoder(send_msg);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);

    // Check connection between client and server
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    // Decode response message and check response message
    list_repo_msg_response_decoder(receive_msg, res_status, res_list_repo);
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(list_repositories, MSG_MAX_LEN);
        strcpy(list_repositories, res_list_repo);
        return STATUS_OK;
    }

    return STATUS_FAILED;
}

int clone_repo(int sockfd, char *repo_name){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char res_repo_name[REPONAME_LEN], res_status[32],
         file_location[FILE_LOCATION_LEN], file_name[FILE_NAME_LEN],
         content[FILE_CONTENT_LEN], pwd[256], absolute_file_path[512],
         absolute_folder_path[256];
    int i;

    // Repository name that you want to clone
    do {
        i = 0;
        bzero(repo_name, sizeof(repo_name));  
        printf("Input your repository name: ");
        while ((repo_name[i++] = getchar()) != '\n');
        repo_name[i-1] = '\0';
        if(repo_name[0] == '\0')
            printf("Repository name can't be blank!");
    }while (repo_name[0] == '\0');

    // Encode and send clone repository request
    clone_repo_msg_request_encoder(send_msg, repo_name);
    write(sockfd, send_msg, sizeof(send_msg));
    do {
        // Memory set for variables
        bzero(receive_msg, MSG_MAX_LEN);
        bzero(res_repo_name, sizeof(res_repo_name));
        bzero(res_status, sizeof(res_status));
        bzero(file_location, sizeof(file_location));
        bzero(file_name, sizeof(file_name));
        bzero(content, sizeof(content));
        bzero(absolute_file_path, sizeof(absolute_file_path));
        bzero(absolute_folder_path, sizeof(absolute_folder_path));

        // Check connection between client and server    
        if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
            printf("Disconnect from server! Client forced to quit!\n");
            return SERVER_DISCONNECT;   
        }
        
        // Decode response message and check for response status
        clone_repo_msg_response_decoder(receive_msg, res_status, res_repo_name,
                                        file_location, file_name, content);

        if(strcmp(res_status, SENDING) == 0){
            // get absolute path to current folder
            get_pwd(pwd);
            sprintf(absolute_folder_path, "%s/%s%s", pwd, res_repo_name, file_location);
            sprintf(absolute_file_path, "%s/%s", absolute_folder_path, file_name);

            // start writing content to file
            append_file_content(absolute_file_path, absolute_folder_path, content);

            continue;
        } else if (strcmp(res_status, COMPLETED) == 0){
            return STATUS_OK;
        } else {
            return STATUS_FAILED;
        }       
    } while (1);
}

int logout(int sockfd) {
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char res_status[RESPONSE_STATUS_LEN+1];

    // Encode and send logout message
    logout_msg_request_encoder(send_msg);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);

    // Check connection between client and server    
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    // Decode message and check for response status
    logout_msg_response_decoder(receive_msg, res_status);
    if(strcmp(res_status, RESPONSE_OK) == 0){
        return STATUS_OK;
    }

    return STATUS_FAILED;
}