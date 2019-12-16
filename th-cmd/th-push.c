#include "../helper/transfer_handler.h"
#include "../helper/socket_validate.h" 
#include "../helper/network_config.h"

#define LOGINED 1
#define NOT_LOGIN 0
#define STATUS_OK 1
#define STATUS_FAILED 0
#define SERVER_DISCONNECT -1

void run(int sockfd);       // Start processing progress between server and client

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
    char send_msg[MSG_MAX_LEN], receive_msg[MSG_MAX_LEN];
    char pwd[MSG_MAX_LEN/2];
    char email[EMAIL_LEN], password[PASSWORD_LEN], username[USERNAME_LEN], 
         res_status[RESPONSE_STATUS_LEN+1], commit[COMMIT_LEN], repo_name[REPONAME_LEN];
    Commit *different_commits, *temp;
    int i, j;

    if(get_repo_name(repo_name) == 0) {
            printf("Can't open commit log! Current position may be the reason\n");
            return;
        };
    
    // Get current repository commit file absolute path
    get_pwd(pwd);
    get_commits_history(pwd, &different_commits);

    printf("Repository: %s", repo_name);

    // Authentication with email and password
    do {
        i = 0;
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

    // Encode and send authentication request
    auth_msg_request_encoder(send_msg, email, password);
    write(sockfd, send_msg, MSG_MAX_LEN);
    bzero(receive_msg, MSG_MAX_LEN);

    // Check if server is working or not
    if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
        printf("Disconnect from server! Client forced to quit!\n");
        free_commit_nodes(&different_commits);
        return;   
    }
    
    auth_msg_response_decoder(receive_msg, res_status, username);

    // If authentication success, compare commits betwwen server and local
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(send_msg, sizeof(send_msg));

        // Get the different commit between client and server
        get_server_commits_msg_request_encoder(send_msg, repo_name);
        write(sockfd, send_msg, sizeof(send_msg));
        do {
            bzero(receive_msg, MSG_MAX_LEN);
            bzero(commit, sizeof(commit));
            bzero(res_status, sizeof(res_status));

            // Check connection betweeen server and client
            if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
                printf("Disconnect from server! Client forced to quit!\n");
                return;   
            }

            // Decode response message  
            get_server_commits_msg_response_decoder(receive_msg, res_status, commit);
            if(strcmp(res_status, SENDING) == 0){
                // Remove same commit from list of server-local different commits
                different_commits = remove_data_node(different_commits, commit);
                continue;
            } else if (strcmp(res_status, COMPLETED) == 0){
                // Break loop if server completely sent all server's repository commits 
                printf("break\n");
                break;
            } else {
                // Uhmhh, something went wrong that I don't know
                printf("Something went wrong!\n");
                free_commit_nodes(&different_commits);
                break;
            } 
        } while(1);

        // Push all new commits to server
        char absolute_commit_path[MSG_MAX_LEN];
        temp = different_commits;
        repo_name[strlen(repo_name)-1] = '\0';
        while(temp != NULL){
            sprintf(absolute_commit_path, "%s/.th/commits/%s", pwd, temp->commit_name);
            transfer_a_commit(sockfd, repo_name, temp, absolute_commit_path);
            temp = temp->next;
        }
        printf("Push completed\n");
    } else {
        printf("ERROR: Email or password was wrong!\n");
    }
    free_commit_nodes(&different_commits);
    return;
}
