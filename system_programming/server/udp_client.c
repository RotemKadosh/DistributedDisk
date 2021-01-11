
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include <stdlib.h>/*atoi*/
#include <assert.h> /*assert*/
#include "udp.h"  

#define MAXLINE (5) 
#define TRUE (1) 

#define UDP_PORT_NUM (4213)  
int main() 
{    
    char buffer[5] = {0}; 
    char *message = "ping"; 
    int sock_fd = 0;
    int n = 0; 
    sockaddr_t serv_addr = {0}; 
    socklen_t len = sizeof(serv_addr);
    sock_fd = CreateUdpSocket(); 
    if(FAIL == sock_fd)
    {
        perror("CreateUdpSocket failed");
        return FAIL;
    }  
    InitUdpSockAddr(&serv_addr, htonl(INADDR_LOOPBACK), UDP_PORT_NUM); 

    while(TRUE)
    {   
        if(FAIL == sendto(sock_fd, message, MAXLINE, 0, (struct sockaddr*)&serv_addr, len))
        {
            close(sock_fd); 
            perror("sendto failed");
            return FAIL; 
        } 
        sleep(4);    
        n = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &len); 
        if(FAIL == n)
        {
            close(sock_fd); 
            perror("rcvfrom failed");
            return FAIL; 
        }

        buffer[n] = '\0'; 
        printf("pong received: %s\n", buffer); 
        
    }
 

    close(sock_fd); 
    return SUCCESS;
} 
