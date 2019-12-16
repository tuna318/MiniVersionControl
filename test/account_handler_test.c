#include "../helper/account_handler.h"

void checkSignUpNew()
{
    printf("-------sign up success-----------\n");
    char *name = createAccount("hanh.shin.1998@gmail.com", "mmm", "123456");
    if (name == NULL)
        printf("FAILED\n");
    else if (strcmp(name, "mmm") == 0)
    {
        printf("%s\nSUCCESS\n", name);
    }
    else
    {
        printf("FAILED\n");
    }
}

void checkSignUpExisted()
{
    printf("-------sign up with user name existed-----------\n");
    char *name;
    name = createAccount("hanh.shin.1998@gmail.com", "hanhnguyen", "123456");
    if (name == NULL)
        printf("SUCCESS\n");
    else
        printf("FAILED\n");
}

void checkLoginOK()
{
    printf("------- login success -----------\n");
    const char *name = loginAuth("hanh.shin.1998@gmail.com", "123456");
    if (strcmp(name, "hanhnguyen") == 0)
        printf("%s\nSUCCESS\n", name);
    else
        printf("FALIED\n");
}

void checkLoginFail()
{
    printf("------- login fail with existed user name -----------\n");
    const char *name = loginAuth("hihi.com", "123456");
    if (name == NULL)
        printf("SUCCESS\n");
    else
        printf("FALIED\n");
}

void checkCreateRepoSuccess()
{
    printf("------- Create repository success -----------\n");
    char *name = createRepo("tuna", "linux");
    if (name == NULL)
        printf("FAILED\n");
    else if (strcmp(name, "linux") == 0)
        printf("SUCCESS\n");
    else
        printf("FALIED\n");
}

void checkCreateRepoFail()
{
    printf("------- Create repository fail with existed repo ------------\n");
    char *name = createRepo("hanhnguyen", "mini-network");
    if (name == NULL)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILED\n");
    }
}

void checkIsRepoExisted()
{
    printf("------- Repository existed -----------\n");
    char *name = isExistingRepo("hanhnguyen", "network-check");
    if (name == NULL)
        printf("FAILED\n");
    else if (strcmp(name, "network-check") == 0)
        printf("SUCCESS\n");
    else
        printf("FALIED\n");
}

void checkIsRepoNotExist()
{
    printf("------- Repository does not exist -----------\n");
    char *name = isExistingRepo("hanhnguyen", "shinshin");
    if (name == NULL)
        printf("SUCCESS\n");
    else
        printf("FAILED\n");
}

void checkListRepoSuccess()
{
    printf("------- User has repositories -----------\n");
    char *name = listRepo("tuna");
    if (name == NULL)
        printf("FAILED\n");
    else if (strcmp(name, "miniversion\nlinux\n") == 0)
        printf("SUCCESS\n");
    else
        printf("FALIED\n");
}

void checkListRepoNoExist()
{
    printf("------- Repository does not exist -----------\n");
    char *name = listRepo("hihi");
    if (name == NULL)
        printf("SUCCESS\n");
    else
        printf("FAILED\n");
}

int main()
{
    checkSignUpExisted();
    checkSignUpNew();
    checkLoginOK();
    checkLoginFail();
    checkCreateRepoSuccess();
    checkCreateRepoFail();
    checkIsRepoExisted();
    checkIsRepoNotExist();
    checkListRepoSuccess();
    checkListRepoNoExist();
}