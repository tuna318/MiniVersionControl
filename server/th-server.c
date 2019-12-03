#include "../helper/socket_validate.h" 
#include "../helper/server_request_handler.h"

void run(int sockfd);      
void func(int signum); 

int main() {

    struct sockaddr_in servaddr, cliaddr; 
    int sockfd, connfd, len; 
    char port[20];
    int pid;

    sprintf(port, "%d", SERVER_PORT);
      
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(SERVER_PORT); 
    
    // Bind the socket with the server address 
    if ((bind(sockfd, (const struct sockaddr *)&servaddr,  sizeof(servaddr))) < 0 ){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    if ((listen(sockfd, 5)) != 0) { 
            printf("Listen failed...\n"); 
            exit(0); 
        }

    // Now server is ready to listen and verification 
    do {
        printf("Waiting for new client\n");
        memset(&cliaddr, 0, sizeof(cliaddr));

        len = sizeof(cliaddr);

        // Accept the data packet from client and verification 
        connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
        if (connfd < 0) { 
            printf("server acccept failed...\n"); 
            exit(0); 
        }

        pid = fork();
        // Accept success, start process data  
        if(pid == 0){
            run(connfd);
            close(connfd);
            kill(getpid(), SIGTERM);
        }

    } while (1);
    
    close(sockfd);      
    return 0; 
} 

void run(int sockfd) {
    int login_flag = 0;
    char buffer[MSG_MAX_LEN], msg_flag[MSG_FLAG_LEN+1];
    char recv_msg[MSG_MAX_LEN], send_msg[MSG_MAX_LEN];
    char email[EMAIL_LEN], username[USERNAME_LEN], password[PASSWORD_LEN];

    do {
        bzero(buffer, MSG_MAX_LEN);
        // Read the request message from client to buffer
        if((read(sockfd, buffer, sizeof(buffer))) <= 0)
            return;

        printf("buffer:\n %s\n", buffer);

        // Get the msg flag
        memset(msg_flag, '\0', sizeof(msg_flag));
        strncpy(msg_flag, buffer, 2);

        if(strcmp(msg_flag, LOGIN_FLAG) == 0) {
            login_flag = login_handler(sockfd, buffer, email, password);
        } else if (strcmp(msg_flag, SIGNUP_FLAG) == 0) {
            signup_handler(sockfd, buffer);
        } else if (strcmp(msg_flag, CREATE_REPO_FLAG) == 0 && login_flag == 1) {
            create_repo_handler(sockfd, buffer, username, email);
        } else if (strcmp(msg_flag, LIST_REPO_FLAG) == 0 && login_flag == 1) {
            list_repo_handler(sockfd, buffer, username);
        } else if (strcmp(msg_flag, LOGOUT_FLAG) == 0 && login_flag == 1) {
            login_flag = logout_handler(sockfd, buffer);
            if (login_flag == 0) {
                bzero(email, sizeof(email));
                bzero(username, sizeof(username));
                bzero(password, sizeof(password));
            }
        } else if (strcmp(msg_flag, CLONE_REPO_FLAG) == 0 && login_flag == 1) {
            clone_repo_handler(sockfd, buffer, username);
        } else {
            write(sockfd, CHECK_CONNECTION, strlen(CHECK_CONNECTION));
            return;
        }
    } while(1);
    
    return;
}

void func(int signum) 
{ 
    if(wait(NULL) == signum)
        kill(signum, SIGTERM); 
} 
