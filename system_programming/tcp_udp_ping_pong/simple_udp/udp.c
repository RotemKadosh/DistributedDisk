#include "udp.h"

#include <sys/socket.h> /*socket*/
#include <string.h> /*memset*/

#define PORT (5000)

int CreateUdpSocket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

void InitUdpSockAddr(sockaddr_t *sockadd, in_addr_t address)
{
    memset(sockadd, 0 , sizeof(sockaddr_t));
    sockadd->sin_addr.s_addr = address; 
    sockadd->sin_port = htons(PORT); 
    sockadd->sin_family = AF_INET; 
}