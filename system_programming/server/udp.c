#include "udp.h"

#include <sys/socket.h> /*socket*/
#include <string.h> /*memset*/



int CreateUdpSocket()
{
    return socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK , 0);
}

void InitUdpSockAddr(sockaddr_t *sockadd, in_addr_t address, int port_num)
{
    memset(sockadd, 0 , sizeof(sockaddr_t));
    sockadd->sin_addr.s_addr = address; 
    sockadd->sin_port = htons(port_num); 
    sockadd->sin_family = AF_INET; 
}