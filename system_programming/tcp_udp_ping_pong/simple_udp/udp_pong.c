
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include <stdlib.h>/*atoi*/
#include <assert.h> /*assert*/
#include "udp.h"  

#define MAXLINE (5) 
   
int main(int argc, char **argv) 
{    
    char buffer[5] = {0}; 
    char *message = "pong"; 
    int sock_fd = 0;
    int n = 0; 
    sockaddr_t serv_addr = {0}; 

    assert(1 > argc);

    sock_fd = CreateUdpSocket(); 
    if(FAIL == sock_fd)
    {
        perror("CreateUdpSocket failed");
        return FAIL;
    }  
    
    InitUdpSockAddr(&serv_addr, htonl(INADDR_LOOPBACK), atoi(argv[1])); 


    if(FAIL == sendto(sock_fd, message, MAXLINE, 0, (struct sockaddr*)NULL, NULL))
    {
        close(sock_fd); 
        perror("sendto failed");
        return FAIL; 
    } 
     
    n = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
    if(FAIL == n)
    {
        close(sock_fd); 
        perror("rcvfrom failed");
        return FAIL; 
    }

    buffer[n] = '\0'; 
    printf("pong received: %s\n", buffer); 
  

    close(sock_fd); 
    return SUCCESS;
} 
