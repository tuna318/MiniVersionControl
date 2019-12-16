#include "file_handler.h"
#include <sqlite3.h>


#define FAIL_DB "connect to db failed"
#define ARR_NUMBER 50
//define db and SQL statement
sqlite3 *db;
sqlite3_stmt *res;

//init db


//login 
char *loginAuth(char *email, char *password);

//signUp
char *createAccount(char *email, char *name, char *password);

//logout
void logout();

//create repo
char *createRepo(char *userName, char *nameRepo);

//list Repo
char *listRepo(char *userName);

//clone Repo
char *isExistingRepo(char *userName, char *nameRepo);