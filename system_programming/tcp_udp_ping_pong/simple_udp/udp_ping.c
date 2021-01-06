
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include "udp.h"

#define MAXLINE 5


int main() 
{    
    char buffer[5]; 
    char *message = "ping"; 
    int listen_fd = 0;
    int n = 0;
    socklen_t len = 0; 
    sockaddr_t servaddr, cliaddr; 
    
    

    listen_fd = CreateUdpSocket();
    if(FAIL == listen_fd)
    {
        return FAIL;
    }  
    InitUdpSockAddr(&servaddr, htonl(INADDR_ANY));


    if(FAIL == bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        perror("bind failed");
        return FAIL;
    } 
       
    
    len = sizeof(cliaddr); 

    n = recvfrom(listen_fd, buffer, sizeof(buffer) -1 , 0, (struct sockaddr*)&cliaddr, &len); 
    if(FAIL == n)
    {
        perror("rcvfrom failed");
        return FAIL;
    }
    buffer[n] = '\0'; 
    printf("ping received: %s\n", buffer); 
           

    sendto(listen_fd, message, MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 

    close(listen_fd);

    return SUCCESS;
} 
