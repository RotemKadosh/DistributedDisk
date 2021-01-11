
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include "udp.h"

#define MAXLINE (27)


int main() 
{    
 
    char *message = "rotem: ping pong broadcast"; 
    int server_fd = 0;
    int n = 1;
    int t = 0;
    socklen_t len = 0; 
    sockaddr_t servaddr = {0}; 
    
    server_fd = CreateUdpSocket();
    if(FAIL == server_fd)
    {
        perror("create failed: ");
        return FAIL;
    }

    if(FAIL == setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, &n, sizeof(n)))
    {
        perror("setsockopt failed: ");
        close(server_fd);
        return FAIL;
    }

    InitUdpSockAddr(&servaddr, htonl(INADDR_BROADCAST));
    while (t++ < 10)
    {
        if(FAIL == sendto(server_fd, message, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)))
        {
            perror("setsockopt failed: ");
            close(server_fd);
            return FAIL;
        } 
    }
    
    close(server_fd);

    return SUCCESS;
} 

