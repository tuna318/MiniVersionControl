#include "../helper/socket_validate.h" 
#include "../helper/server_request_handler.h"

void run(int sockfd);     // Start request processing progress 
void func(int signum);    // Kill process support function

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
    int auth_flag = 0, get_commits_flag = 0, push_status = 0;
    char buffer[MSG_MAX_LEN], msg_flag[MSG_FLAG_LEN+1];
    char recv_msg[MSG_MAX_LEN], send_msg[MSG_MAX_LEN];
    char email[EMAIL_LEN], username[USERNAME_LEN], password[PASSWORD_LEN];

    do {
        bzero(buffer, MSG_MAX_LEN);
        // Read the request message from client to buffer
        if((read(sockfd, buffer, sizeof(buffer))) <= 0)
            return;

        // printf("buffer:\n %s\n", buffer);

        // Get the msg flag
        memset(msg_flag, '\0', sizeof(msg_flag));
        strncpy(msg_flag, buffer, 2);

        // check msg flag and handle request corresponding to it
        if(strcmp(msg_flag, AUTH_FLAG) == 0) {
            /* Handle Authentication request */
            auth_flag = auth_handler(sockfd, buffer, email, username);
        } else if (strcmp(msg_flag, SIGNUP_FLAG) == 0) {
            /* Handle signup request */
            signup_handler(sockfd, buffer);
        } else if (strcmp(msg_flag, CREATE_REPO_FLAG) == 0 && auth_flag == 1) {
            /* Handle create repository request */
            create_repo_handler(sockfd, buffer, username, email);
        } else if (strcmp(msg_flag, LIST_REPO_FLAG) == 0 && auth_flag == 1) {
            /* Handle list of repositories request */
            list_repo_handler(sockfd, buffer, username);
        } else if (strcmp(msg_flag, LOGOUT_FLAG) == 0 && auth_flag == 1) {
            /* Hanle logout request */
            auth_flag = logout_handler(sockfd, buffer);
            // If logout successfully, memset for authenticate variables
            if (auth_flag == 0) {
                bzero(email, sizeof(email));
                bzero(username, sizeof(username));
                bzero(password, sizeof(password));
            }
        } else if (strcmp(msg_flag, CLONE_REPO_FLAG) == 0 && auth_flag == 1) {
            /* Hanle clone repository request */
            clone_repo_handler(sockfd, buffer, username);
        } else if (strcmp(msg_flag, CHECK_COMMITS_FLAG) == 0 && auth_flag == 1) {
            /* Hanle compare commits between server and local request */
            get_commits_flag = get_server_commits_handler(sockfd, buffer, username);
        } else if (strcmp(msg_flag, PUSH_FLAG) == 0 && auth_flag == 1 && get_commits_flag == 1) {
            /* Handle push commits from client */
            push_status = push_commits_handler(sockfd, buffer, username);
            if (push_status ==  1)
                get_commits_flag = 1;
            else 
                get_commits_flag = 0;
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
