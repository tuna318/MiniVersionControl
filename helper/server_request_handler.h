/*******************
* SERVER REQUEST HANDLER
********************
*
* Hanlder request for server and more
*
********************/

#include "../helper/message_handler.h"
#include "../helper/network_config.h"
#include "../helper/file_handler.h"

/* < Authentication Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
* @email (return value): store email return from handler
* @username (return value): store username return from handler
*/
int auth_handler(int sockfd, char* buffer, char* email, char* username);

/* < Signup Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
*/
void signup_handler(int sockfd, char* buffer);

/* < Create Repository Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
*/
void create_repo_handler(int sockfd, char* buffer, char *username, char*email);

/* < List of Repository Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
* @username (input): parse by server, use to query database
*/
void list_repo_handler(int sockfd, char* buffer, char *username);

/* < Logout Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
*/
int logout_handler(int sockfd, char* buffer);

/* < Clone Repository Handler >
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
* @username (input): parse by server, use to query database
*/
void clone_repo_handler(int sockfd, char* buffer, char *username);

/* < Compare Commits between Server and Client Handler >
* 
* @sockfd (input): socket file descriptor
* @buffer (input): request buffer sent from client
*/
void check_new_commits_handler(int sockfd, char* buffer, char* username);

/* < Get location path of MiniVersionControl >
* @path (return value): location path of MiniVersionControl
*/
void get_main_folder_location(char *path);

/* < Transfer a folder to client >
* @sockfd (input): socket file descriptor
* @folder_absolute_path (input): absolute path to transfer folder
* @folder_name: (input): name of transfer folder 
*/
int transfer_a_folder(int sockfd, char* folder_absolute_path, char* folder_name);