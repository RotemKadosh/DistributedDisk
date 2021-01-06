
#include <stdio.h> /*printf*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*close*/
#include "udp.h"  

#define MAXLINE 1000 
   
int main() 
{    
    char buffer[5]; 
    char *message = "pong"; 
    int sock_fd = 0;
    int n = 0; 
    sockaddr_t serv_addr; 

    sock_fd = CreateUdpSocket(); 
    if(FAIL == sock_fd)
    {
        perror("CreateUdpSocket failed");
        return FAIL;
    }  
    

    InitUdpSockAddr(&serv_addr, htonl(INADDR_LOOPBACK)); 


    if(FAIL == connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) 
    { 
        perror("connect failed");
        return FAIL;
    } 
  

    if(FAIL == sendto(sock_fd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(serv_addr)))
    {
        perror("sendto failed");
        return FAIL; 
    } 
     
    n = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
    if(FAIL == n)
    {
        perror("rcvfrom failed");
        return FAIL; 
    }

    buffer[n] = '\0'; 
    printf("pong received: %s\n", buffer); 
  

    close(sock_fd); 
    return SUCCESS;
} 
