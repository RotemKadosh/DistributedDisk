#include <stdio.h> /*printf perror*/
#include <unistd.h>/*rad write close*/
#include <sys/socket.h> /*bind connect*/
#include <string.h> /*memset*/
#include <stdlib.h> /*atoi*/
#include <assert.h> /*assert*/

#include "tcp.h"

#define MAX (10)



int WriteReadLoop(int sock_fd) 
{ 
    char buff[MAX]; 
    int n = 10; 
    char msg[] = "pong";
    while(n-- > 0)
    { 
        memset(buff, 0, MAX);
        if(FAIL == write(sock_fd, msg, sizeof(msg)))
        {
            printf("write failed\n");
            return FAIL;
        } 
        if(FAIL == read(sock_fd, buff, sizeof(buff)))
        {
            printf("read failed\n");
            return FAIL;
        }  
        printf("received : %s\n", buff);  
    } 
    return SUCCESS;
} 
  
int main(int argc, char **argv) 
{ 
    int sock_fd = 0; 
    sockaddr_t serv_addr = {0}; 
  
    assert(1 > argc);
    sock_fd = CreateTcpSocket(); 
    if (FAIL == sock_fd) 
    { 
        perror("socket creation failed\n"); 
        return FAIL; 
    } 
    InitTcpSockAddr(&serv_addr, htonl(INADDR_LOOPBACK), atoi(argv[1]));
     
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