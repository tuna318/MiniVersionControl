#include "../helper/socket_validate.h" 
#include "../helper/network_config.h"
#include "../helper/message_handler.h"

#define LOGINED 1
#define NOT_LOGIN 0
#define STATUS_OK 1
#define STATUS_FAILED 0
#define SERVER_DISCONNECT -1

void run(int sockfd);                                   // Start the menu
int login(int sockfd, char* username);                  // Login function
void logined_menu(int sockfd, char *username);          // Send text function
int sign_up(int sockfd, char *username);                // Signup handle function

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
    int login_flag = 0;

    do {
        printf("Logined as %s\n", username);
        printf("------------------------------------------\n");
        printf("1. Create a repository\n");
        printf("2. List repositories\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        printf("Your choice: ");
        scanf("%c%*c", &selector);

        switch(selector){
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
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

    login_msg_request_encoder(send_msg, email, password);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    login_msg_response_decoder(receive_msg, res_status, res_username);

    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(username, USERNAME_LEN);
        strcpy(username, res_username);
        return 1;
    } 

    return 0;
}

int sign_up(int sockfd, char *username){
    char send_msg[MSG_MAX_LEN];
    char receive_msg[MSG_MAX_LEN];
    char email[EMAIL_LEN], password[PASSWORD_LEN], password_confirmation[PASSWORD_LEN],
         res_username[USERNAME_LEN], res_status[RESPONSE_STATUS_LEN+1];
    int i;

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
    

    signup_msg_request_encoder(send_msg, username, email, password);
    write(sockfd, send_msg, sizeof(send_msg));
    bzero(receive_msg, MSG_MAX_LEN);
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        return SERVER_DISCONNECT;   
    }
    
    signup_msg_response_decoder(receive_msg, res_status, res_username);

    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(username, USERNAME_LEN);
        strcpy(username, res_username);
        return 1;
    }

    return 0;
}