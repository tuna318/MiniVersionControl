#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#define FAIL_DB "connect to db failed"
#define ARR_NUMBER 50
//define db and SQL statement
sqlite3 *db;
sqlite3_stmt *res;

//init db


//login 
const char *loginAuth(char *email, char *password);

//signUp
const char *createAccount(char *email, char *name, char *password);

//logout
void logout();

//create repo
const char *createRepo(char *userName, char *nameRepo);

//list Repo
char *listRepo(char *email);

//clone Repo
const char *isExistingRepo(char *userName, char *nameRepo);