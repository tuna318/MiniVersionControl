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
    Commit *local_commits, *temp;
    int i, j;

    if(get_repo_name(repo_name) == 0) {
            printf("Can't open commit log! Current position may be the reason\n");
            return;
        };
    
    // Get current repository commit file absolute path
    get_pwd(pwd);
    get_commits_history(pwd, &local_commits);

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
        free_commit_nodes(&local_commits);
        return;   
    }
    
    auth_msg_response_decoder(receive_msg, res_status, username);

    // If authentication success, compare commits betwwen server and local
    if(strcmp(res_status, RESPONSE_OK) == 0){
        bzero(send_msg, sizeof(send_msg));

        // Request to send local commit logs
        define_pull_repo_msg_request_encoder(send_msg, PULL_FLAG, repo_name);
        write(sockfd, send_msg, MSG_MAX_LEN);
        bzero(receive_msg, MSG_MAX_LEN);

        // Check if server is working or not
        if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
            printf("Disconnect from server! Client forced to quit!\n");
            free_commit_nodes(&local_commits);
            return;   
        }

        define_pull_repo_msg_response_decoder(receive_msg, res_status);
        if(strcmp(res_status, RESPONSE_OK) == 0) {
            // Send local commit to server
            temp = local_commits;
            while(temp != NULL) {
                bzero(send_msg, sizeof(send_msg));
                send_local_commits_msg_request_encoder(send_msg, SENDING, temp->commit_name);
                write(sockfd, send_msg, sizeof(send_msg));
                temp = temp->next;

            }
            send_local_commits_msg_request_encoder(send_msg, COMPLETED, "");
            write(sockfd, send_msg, sizeof(send_msg));
        }
        
        // Get commit data from server and processing
        char res_repo[REPONAME_LEN], commit_name[COMMIT_LEN], relative_folder[MSG_MAX_LEN/4],
             file_name[64], content[MSG_MAX_LEN/2], absolute_file_path[MSG_MAX_LEN/4],
             absolute_folder_path[MSG_MAX_LEN/4];
        get_pwd(pwd);
        do {
            // check connection
            if(read(sockfd, receive_msg, sizeof(receive_msg)) <= 0){
                printf("Disconnect from server! Client forced to quit!\n");
                free_commit_nodes(&local_commits);
                return;   
            }
            // Decode the message from server
            send_commits_request_decoder(receive_msg, res_status, res_repo, 
                                 commit_name, relative_folder, file_name, content);
            if(strcmp(res_status, SENDING) == 0 ){
                sprintf(absolute_folder_path, "%s/.th/commits/%s%s", pwd, commit_name, relative_folder);
                sprintf(absolute_file_path, "%s/%s", absolute_folder_path, file_name);
                printf("abs_path: %s\n", absolute_file_path);
                // start writing content to file
                append_file_content(absolute_file_path, absolute_folder_path, content);
                continue;
            } else if (strcmp(res_status, COMPLETED) == 0) {
                char commit_file_path[MSG_MAX_LEN/4];

                sprintf(commit_file_path, "%s/.th/%s", pwd, COMMIT_FILE);
                addCommitToLog(commit_file_path, content);

                continue;
            } else {
                break;
            }
        } while (1);
        printf("Pull completed\n");
    }
    free_commit_nodes(&local_commits);
    return;
}
