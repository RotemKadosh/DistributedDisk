#include <sys/select.h>/* select, fd_set*/
#include <stdio.h> /*perror*/
#include <string.h>/*strcmp*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*read write*/
#include "server.h" /*createUdpSocket, InitUdpAdd*/

#include <stdlib.h>/*fopen fclose*/
#include <assert.h> /*assert*/
#include <fcntl.h>/*open*/

typedef int FD;
#define STDIN (0)
#define TCP_IDX (0)
#define UDP_IDX (1)
#define STDIN_IDX (2)
#define TCP_PORT_NUM (5000)
#define UDP_PORT_NUM (5001)
#define REQUEST_BUFF_SZ (5)
#define MAX_LINE (5)
#define QUIT (-2)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define NO_FILES (0)
#define FALSE (0)
#define TRUE (0)



int Log(FD log, const char *msg, size_t size)
{
   
    assert(NULL != msg);
    return write(log, msg, size);
}

FD SetUpUdpSock(sockaddr_t *server)
{
    FD udp_sock = CreateUdpSocket();
    if(FAIL == udp_sock)
    {
        perror("create socket failed");
        return FAIL;
    }  
    InitUdpSockAddr(server, htonl(INADDR_ANY), UDP_PORT_NUM);


    if(FAIL == bind(udp_sock, (struct sockaddr*)server, sizeof(server)))
    {
        close(udp_sock);
        perror("bind failed");
        return FAIL;
    } 
    return udp_sock;
}

FD SetUpTcpSock(sockaddr_t *server)
{
    FD sock_fd = CreateTcpSocket(); 
    if (FAIL == sock_fd) 
    { 
        perror("socket creation failed\n"); 
        return FAIL; 
    } 
    InitTcpSockAddr(server, htonl(INADDR_ANY), TCP_PORT_NUM);
  
    if (FAIL == bind(sock_fd, (struct sockaddr *)&server, sizeof(server))) 
    { 
        close(sock_fd);
        perror("bind failed\n"); 
        return FAIL;  
    } 

    if (FAIL == listen(sock_fd, REQUEST_BUFF_SZ)) 
    { 
        close(sock_fd);
        perror("listen failed\n"); 
        return FAIL;  
    } 
    return sock_fd;  
}

void CleanUP(FD *arr, FD log)
{
    int i = 0;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        if(0 < arr[i])
        { 
          close(arr[i]);
        } 
    }
    Log(log, "clean up\n", 10);
    close(log);
}
int ErrorHandling(FD *arr,FD log ,const char *message)
{
    Log(log, "error handling\n",16);
    CleanUP(arr, log);
    perror(message);
    return FAIL;
}

FD SetSelectArgs(fd_set *set, FD *conn_arr, struct timeval * timeout)
{
    int i = 0;
    FD max = 0;
    FD_ZERO(set);
    
    timeout->tv_sec = 7;
    timeout->tv_usec = 0;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        if( -1 != conn_arr[i])
        { 
            max = MAX(max, conn_arr[i]);
            FD_SET(conn_arr[i], set);
        } 
    }
    return max + 1;
}

void InsertToArr(FD new_conn,FD *arr)
{
    int i = 0;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        if(-1 == arr[i])
        { 
           arr[i] = new_conn;
           break;
        } 
    }   
}

int IsQuit(char *buffer)
{
    return !strcmp(buffer, "quit");
}

int TalkToUdpClient(FD udp_fd)
{
    char buffer[MAX_LINE] = {0};
    sockaddr_t cliaddr = {0};
    char *msg = "pong";
    int n = recvfrom(udp_fd, buffer, sizeof(buffer) - 1 , 0, (struct sockaddr*)&cliaddr, NULL); 
    if(FAIL == n)
    {
        perror("rcvfrom failed");
        return FAIL;
    }
    buffer[n] = '\0';
    printf("ping received: %s\n", buffer); 
    if(FAIL == sendto(udp_fd, msg, MAX_LINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)))
    {
        perror("sendto failed");
        return FAIL;
    }
    return SUCCESS;
}

int TalkToClients(FD *conn_fds, fd_set *read_set, int n_ready)
{
    int i = 0;
    int n = 0;
    FD fd = 0;
    char buffer[MAX_LINE] = {0};
    char msg[] = "pong";
    for(i = 0; 0 < n_ready || i < FD_SETSIZE; ++i)
    {
        fd = conn_fds[i];
        if(fd != 0 && FD_ISSET(fd, read_set)) 
        {
            memset(buffer, 0, MAX_LINE);
            --n_ready;
            n = read(fd, buffer, sizeof(buffer) - 1);
            if (0 > n)
            {
                buffer[n] = '\0';
                printf("server received from fd %d: %s\n",fd, buffer); 
            }
            else if(0 == n)
            {
                close(fd);
                conn_fds[i] = 0;
                continue;
            }
            else
            {
                return FAIL;
            }
            n = write(fd, msg, sizeof(msg));
            if(FAIL == n)
            {
                return FAIL;
            }
        }
    }
    return SUCCESS;
}



int TalkToStdin(FD std_in)
{
    int n = 0;
    char buffer[MAX_LINE] = {0};
    char msg[] = "pong";
    memset(buffer, 0, MAX_LINE);
    n = read(std_in, buffer, sizeof(buffer) - 1);
    if(0 > n)
    {
        buffer[n] = '\0';
        printf("server received from stdin : %s\n", buffer); 
    }
    if(IsQuit(buffer))
    {
        return QUIT;
    }
    if(0 == strcmp(buffer, "ping"))
    {
        n = write(std_in, msg, sizeof(msg));
        if(FAIL == n)
        {
            return FAIL;
        }
    }
    return SUCCESS;
}
FD OpenLog()
{
    return open("./log.txt", O_CREAT | O_WRONLY);
}

int main()
{
    sockaddr_t tcp_add = {0}, udp_add = {0};
    FD conn_fds[FD_SETSIZE] = {0};
    int stop_flag = FALSE;
    fd_set read_set = {0};
    int max = 0;
    int ready_fd = 0;
    FD new_conn = 0;
    FD udp_sock = 0;
    FD tcp_sock = 0;
    int status = 0;
    struct timeval timeout = {7,0};
    FD log  = -1;

    if(FAIL == OpenLog(log))
    {
        perror("open log Failed");
        return FAIL;
    }
    /*todo - setup connection*/
    tcp_sock = SetUpTcpSock(&tcp_add);
    if(FAIL == tcp_sock)
    {
        perror("tcp_sock Failed");
    }
    conn_fds[TCP_IDX] = tcp_sock;

    udp_sock = SetUpUdpSock(&udp_add);
    if(FAIL == udp_sock)
    {
        close(tcp_sock);
        perror("udp_sock Failed");
        return FAIL;
    }
    conn_fds[UDP_IDX] = udp_sock;
    conn_fds[STDIN_IDX] = STDIN;
    /*----------------------*/
    while(!stop_flag)
    {
        /*TODO - take out to event loop func*/
        max = SetSelectArgs(&read_set, conn_fds, &timeout);

        ready_fd = select(max, &read_set, NULL, NULL, &timeout);
        if(FAIL == ready_fd)
        {
            return ErrorHandling(conn_fds, log,"select fail\n");
        }
        if(NO_FILES == ready_fd)
        {
            Log(log, "noting happened\n",17);
            continue;
        }
        /*TODO - take out to HandleStdin*/
        if(FD_ISSET(conn_fds[STDIN_IDX], &read_set))
        {
            if(QUIT == TalkToStdin(conn_fds[UDP_IDX]))
            {
                stop_flag = TRUE;
                break;
            }
        }
        /*TODO - take out to HandleNewReq*/
        if(FD_ISSET(conn_fds[TCP_IDX], &read_set))
        {
            --ready_fd;
            new_conn = accept(conn_fds[TCP_IDX], NULL, NULL);
            if(FAIL == new_conn)
            {
                return ErrorHandling(conn_fds, log,"accept failed");
            }

            InsertToArr(new_conn, conn_fds);
        }
        /*TODO - take out to HandleUDP*/
        if(FD_ISSET(conn_fds[UDP_IDX], &read_set))
        {
            --ready_fd;
            status = TalkToUdpClient(conn_fds[UDP_IDX]);
            if(FAIL == status)
            {
                return ErrorHandling(conn_fds, log, "talk to udp client Faild");
            }
        }
        status = TalkToClients(conn_fds, &read_set, ready_fd);
        if(FAIL == status)
        {
            return ErrorHandling(conn_fds, log , "talk to client Faild");
        }  
    }
    CleanUP(conn_fds, log);

    return SUCCESS;

}
