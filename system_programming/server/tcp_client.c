#include <stdio.h> /*printf perror*/
#include <unistd.h>/*rad write close*/
#include <sys/socket.h> /*bind connect*/
#include <string.h> /*memset*/
#include <stdlib.h> /*atoi*/
#include <assert.h> /*assert*/

#include "tcp.h"

#define MAX (10)
#define TCP_PORT_NUM (6543)
#define TRUE (1)


int WriteReadLoop(int sock_fd) 
{ 
    char buff[MAX]; 
    int n = 10; 
    char msg[] = "ping";
    while(TRUE)
    { 
        sleep(6);
        memset(buff, 0, MAX);
        if(FAIL == write(sock_fd, msg, sizeof(msg)))
        {
            printf("write failed\n");
            return FAIL;
        } 
        sleep(1);
        if(FAIL == read(sock_fd, buff, sizeof(buff)))
        {
            printf("read failed\n");
            return FAIL;
        }  
        printf("received : %s\n", buff);  
    } 
    return SUCCESS;
} 
  
int main() 
{ 
    int sock_fd = 0; 
    sockaddr_t serv_addr = {0}; 
  
 
    sock_fd = CreateTcpSocket(); 
    if (FAIL == sock_fd) 
    { 
        perror("socket creation failed\n"); 
        return FAIL; 
    } 
    InitTcpSockAddr(&serv_addr, htonl(INADDR_LOOPBACK), TCP_PORT_NUM);
 
         
    if (FAIL == connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    { 
        close(sock_fd); 
        perror("connect failed\n"); 
        return FAIL; 
    } 

    if(FAIL == WriteReadLoop(sock_fd))
    {
        close(sock_fd);
        return FAIL;
    }
    
    close(sock_fd); 
    return SUCCESS;
} 