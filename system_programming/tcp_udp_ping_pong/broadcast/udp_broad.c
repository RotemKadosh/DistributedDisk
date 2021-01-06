

#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include "udp.h"

#define MAXLINE 5


int main() 
{    
 
    char *message = "rotem: ping pong broadcast"; 
    int server_fd = 0;
    int n = 1;
    socklen_t len = 0; 
    sockaddr_t servaddr; 
    
    server_fd = CreateUdpSocket();
    if(FAIL == server_fd)
    {
        return FAIL;
    }

    if(FAIL == setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, &n, sizeof(n)))
    {
        perror("setsockopt failed: ");
        close(server_fd);
        return FAIL;
    }

    InitUdpSockAddr(&servaddr, htonl(INADDR_BROADCAST));

    sendto(server_fd, message, 27, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

    close(server_fd);

    return SUCCESS;
} 

