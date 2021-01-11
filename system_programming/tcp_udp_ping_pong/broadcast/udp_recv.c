
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include "udp.h"

#define MAXLINE 5


int main() 
{    
    char buffer[30]; 
    char *message = "ping"; 
    int server_fd = 0;
    int n = 0;
    int t = 0;
    socklen_t len = 0; 
    sockaddr_t broad, recvied; 
    
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

    InitUdpSockAddr(&recvied, htonl(INADDR_ANY));

    if(FAIL == bind(server_fd, (struct sockaddr*)&recvied, sizeof(recvied)))
    {
        perror("bind failed");
        return FAIL;
    } 
       
    while(t++ < 0)
    {
        n = recvfrom(server_fd, buffer, sizeof(buffer) - 1 , 0, (struct sockaddr*)NULL, NULL); 
    }
    
    if(FAIL == n)
    {
        perror("rcvfrom failed");
        return FAIL;
    }

    buffer[n] = '\0'; 
    printf("received: %s\n", buffer); 
           

    close(server_fd);

    return SUCCESS;
} 
