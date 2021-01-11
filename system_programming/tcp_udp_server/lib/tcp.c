#include "tcp.h"

#include <sys/socket.h> /*socket*/
#include <string.h> /*memset*/

#define PORT (5000)

int CreateTcpSocket()
{
    return socket(AF_INET, SOCK_STREAM , 0);
}

void InitTcpSockAddr(sockaddr_t *sockadd, in_addr_t address , int port)
{
    memset(sockadd, 0 , sizeof(sockaddr_t));
    sockadd->sin_addr.s_addr = address; 
    sockadd->sin_port = htons(port); 
    sockadd->sin_family = AF_INET; 
}