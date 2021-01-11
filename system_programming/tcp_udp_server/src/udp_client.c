
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include <stdlib.h> /*atoi*/
#include <assert.h> /*assert*/
#include "udp.h"

#define MAXLINE (5)
#define UDP_PORT_NUM (5001)
#define TRUE (1)

int main() 
{    
    char buffer[5] = {0}; 
    char *message = "ping"; 
    int listen_fd = 0;
    int n = 0;
    socklen_t len = 0; 
    sockaddr_t servaddr ={0}, cliaddr = {0}; 
    

    listen_fd = CreateUdpSocket();
    if(FAIL == listen_fd)
    {
        perror("create socket failed");
        return FAIL;
    }  
    InitUdpSockAddr(&servaddr, htonl(INADDR_ANY), UDP_PORT_NUM);


    if(FAIL == bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        close(listen_fd);
        perror("bind failed");
        return FAIL;
    } 
       
    
    len = sizeof(cliaddr); 
    while(TRUE)
    {
        n = recvfrom(listen_fd, buffer, sizeof(buffer) -1 , 0, (struct sockaddr*)&cliaddr, &len); 
        if(FAIL == n)
        {
            close(listen_fd);
            perror("rcvfrom failed");
            return FAIL;
        }
        buffer[n] = '\0'; 
        printf("ping received: %s\n", buffer); 
            

        if(FAIL == sendto(listen_fd, message, MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)))
        {
            close(listen_fd);
            perror("sendto failed");
            return FAIL;
        }
    }
   

    close(listen_fd);

    return SUCCESS;
} 
