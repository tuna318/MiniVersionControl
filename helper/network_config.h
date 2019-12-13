#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <time.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <stdio.h>

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 5002
