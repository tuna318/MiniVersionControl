/*******************
* TRANSFER HANDLER
********************
*
* Inherit from message handler
* Transfer commits folder and more
*
********************/

#include "message_handler.h"
#include "account_handler.h"

int transfer_a_commit(int sockfd, char* repo_name, Commit* commit, char* commit_absolute_path);
