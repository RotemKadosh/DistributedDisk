#include <sys/select.h>/* select, fd_set*/
#include <stdio.h> /*perror*/
#include <string.h>/*strcmp*/
#include <sys/socket.h> /*socket*/
#include <unistd.h> /*read write*/
#include "server.h" /*createUdpSocket, InitUdpAdd*/
#include <stdlib.h>/*fopen fclose*/
#include <assert.h> /*assert*/
#include <fcntl.h>/*open*/


#define STDIN (0)
#define TCP_IDX (0)
#define UDP_IDX (1)
#define STDIN_IDX (2)
#define TCP_PORT_NUM (6543)
#define UDP_PORT_NUM (4213)
#define REQUEST_BUFF_SZ (5)
#define MAX_LINE (5)
#define QUIT (-2)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define NO_FILES (0)
#define FALSE (0)
#define TRUE (0)

typedef int FD;

static FILE *log;
int Log(const char *msg);
FD SetUpUdpSock(sockaddr_t *server);
FD SetUpTcpSock(sockaddr_t *server);
void CleanUP(FD *arr);
int ErrorHandling(FD *arr,const char *message);
FD SetSelectArgs(fd_set *set, FD conn_arr[], struct timeval *timeout);
void InsertToArr(FD new_conn,FD *arr);
int IsQuit(char *buffer);
int TalkToUdpClient(FD udp_fd);
int TalkToClients(FD *conn_fds, fd_set *read_set, int n_ready);
int TalkToStdin(FD std_in);
FILE *OpenLog();
int SetUpSockets(sockaddr_t *tcp_add, sockaddr_t *udp_add,FD conn_fds[FD_SETSIZE]);
int RunServer(FD *conn_fds);
int HandleClients(FD *conn_fds, fd_set *read_set, int *ready_fd);
int HandleUDP(FD *conn_fds, fd_set *read_set, int *ready_fd);
int HandleNewReq(FD *conn_fds, fd_set *read_set, int *ready_fd);
int HandleStdin(FD *conn_fds, fd_set *read_set,int *ready_fd);
int InitServer(FD *conn_fds);
int SetUpBuiltInSockets(FD *conn_fds);


int main(void)
{
    FD conn_fds[FD_SETSIZE] = {0};
    int status = 0;


    if(FAIL == InitServer(conn_fds))
    {
        perror("Init Fail");
    }
    status = RunServer(conn_fds);
    if(status == FAIL)
    {
        perror("run server fail");
    }
    CleanUP(conn_fds);

    return SUCCESS;

}

int Log(const char *msg)
{
   
    assert(NULL != msg);
    return fprintf(log,"%s\n",msg);
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


    if(FAIL == bind(udp_sock, (struct sockaddr*)server, sizeof(*server)))
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
  
    if (FAIL == bind(sock_fd, (struct sockaddr *)server, sizeof(*server))) 
    { 
        close(sock_fd);
        perror("bind failed"); 
        return FAIL;  
    } 

    if (FAIL == listen(sock_fd, REQUEST_BUFF_SZ)) 
    { 
        close(sock_fd);
        perror("listen failed"); 
        return FAIL;  
    } 
    return sock_fd;  
}

void CleanUP(FD *arr)
{
    int i = 0;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        if(0 < arr[i])
        { 
          close(arr[i]);
        } 
    }
    Log("server clean up\n");
    fclose(log);
}

int ErrorHandling(FD *arr,const char *message)
{
    Log("server error handling\n");
    CleanUP(arr);
    perror(message);
    return FAIL;
}

FD SetSelectArgs(fd_set *set, FD conn_arr[], struct timeval *timeout)
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
    return !strcmp(buffer, "quit\n");
}

int TalkToUdpClient(FD udp_fd)
{
    char buffer[10] = {0};
    sockaddr_t cliaddr = {0};
    char *msg = "pong";
    socklen_t len = sizeof(struct sockaddr);
    int n = recvfrom(udp_fd, buffer, sizeof(buffer) - 1 , 0, (struct sockaddr*)&cliaddr, &len); 
    if(FAIL == n)
    {
        perror("rcvfrom failed");
        return FAIL;
    }
    buffer[n] = '\0';
    printf("server received from udp: %s\n", buffer); 
    Log("server received from udp: ping\n");
    if(FAIL == sendto(udp_fd, msg, strlen(msg), MSG_WAITALL, (struct sockaddr *)&cliaddr, len))
    {
        perror("sendto failed");
        return FAIL;
    }
    Log("server send to udp: pong\n");
    return SUCCESS;
}

int HandleTCPClients(FD *conn_fds, fd_set *read_set, int *n_ready)
{
    int i = 0;
    int n = 0;
    FD fd = 0;
    char buffer[10] = {0};
    char msg[] = "pong";
    for(i = 3; 0 < *n_ready && i < FD_SETSIZE; ++i)
    {
        fd = conn_fds[i];
        if(fd != -1 && FD_ISSET(fd, read_set)) 
        {
            memset(buffer, 0, MAX_LINE);
            --*n_ready;
            n = read(fd, buffer, sizeof(buffer) - 1);
            if (0 < n)
            {
                buffer[n] = '\0';
                printf("server received from fd %d: %s\n",fd, buffer); 
                Log("server: received ping from tcp client\n");
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
            Log("server: send pong to tcp client");
        }
    }
    return SUCCESS;
}

int TalkToStdin(FD std_in)
{
    int n = 0;
    char buffer[6] = {0};
    memset(buffer, 0, MAX_LINE);
    n = read(std_in, buffer, sizeof(buffer) - 1);
    if(FAIL == n)
    {
        perror("read from stdin failed");
        return FAIL;
    }
    buffer[n] = '\0';
    printf("server received from stdin : %s\n", buffer); 
    if(IsQuit(buffer))
    {
        Log("server: received quit from stdin\n");
        return QUIT;
    }
    if(0 == strcmp(buffer, "ping\n"))
    {
        Log("server: received ping from stdin\n");
        printf("pong\n");
        Log("server: send pong to stdout\n");
    }
    return SUCCESS;
}

FILE *OpenLog()
{
    return fopen("./log.txt", "w");
}

int SetUpBuiltInSockets(FD *conn_fds)
{
    sockaddr_t tcp_add = {0};
    sockaddr_t udp_add = {0};
    FD udp_sock = 0;
    FD tcp_sock = 0;

    tcp_sock = SetUpTcpSock(&tcp_add);
    if(FAIL == tcp_sock)
    {
        perror("tcp_sock Failed");
        fclose(log);
        return FAIL;
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
    return SUCCESS;
}

int InitServer(FD *conn_fds)
{
    memset(conn_fds, -1, FD_SETSIZE * sizeof(int));
    if(NULL == (log = OpenLog()))
    {
        perror("open log Failed");
        return FAIL;
    }
    return SetUpBuiltInSockets(conn_fds);
  
}

int HandleStdin(FD *conn_fds, fd_set *read_set,int *ready_fd)
{
    if(FD_ISSET(conn_fds[STDIN_IDX], read_set))
    {
        --*ready_fd;
        if(QUIT == TalkToStdin(conn_fds[STDIN_IDX]))
        {
            return QUIT;
        }
    }
    return SUCCESS;
}

int HandleNewReq(FD *conn_fds, fd_set *read_set, int *ready_fd)
{
    int new_conn = -1;
    if(FD_ISSET(conn_fds[TCP_IDX], read_set))
    {
        --*ready_fd;
        new_conn = accept(conn_fds[TCP_IDX], NULL, NULL);
        if(FAIL == new_conn)
        {
            return FAIL;
        }
        Log("new connection is made");
        InsertToArr(new_conn, conn_fds);
    }
    return SUCCESS;
}

int HandleUDP(FD *conn_fds, fd_set *read_set, int *ready_fd)
{
  
    if(FD_ISSET(conn_fds[UDP_IDX], read_set))
    {
        --*ready_fd;
        return TalkToUdpClient(conn_fds[UDP_IDX]);
    }
    return SUCCESS;
}

int HandleClients(FD *conn_fds, fd_set *read_set, int *ready_fd)
{
    int status = 0;
    status = HandleStdin(conn_fds, read_set, ready_fd);
    if(FAIL == status || QUIT == status)
    {
        return status;
    }
    status = HandleNewReq(conn_fds, read_set, ready_fd);
    if(FAIL == status)
    {
        Log("HandleUdp Fail\n");
        return FAIL;
    }
    
    status = HandleUDP(conn_fds, read_set, ready_fd);
    if(FAIL == status)
    {
        Log("HandleUdp Fail\n");
        return FAIL;
    }
    status = HandleTCPClients(conn_fds, read_set, ready_fd);
    if(FAIL == status)
    {
        Log("HandleTCPClients Fail\n");
        return FAIL;
    }
    return SUCCESS;
}

int RunServer(FD *conn_fds)
{
    fd_set read_set = {0};
    int max_fd = 0;
    int ready_fd = 0;
    int stop_flag = FALSE;
    int status = 0;
    struct timeval timeout = {7, 0};
    while(!stop_flag)
    {
        max_fd = SetSelectArgs(&read_set, conn_fds, &timeout);

        ready_fd = select(max_fd, &read_set, NULL, NULL, &timeout);
        if(FAIL == ready_fd)
        {
            return ErrorHandling(conn_fds,"select fail\n");
        }
        if(NO_FILES == ready_fd)
        {
            Log("server: noting happened\n");
            printf("server: noting happened\n");
            continue;
        }
        status = HandleClients(conn_fds, &read_set, &ready_fd);
        if(QUIT == status)
        {
            stop_flag = TRUE;
            break;
        }
        if(FAIL == status)
        {
            Log("server: handle clients failed\n");
            return ErrorHandling(conn_fds, "handle clients failed");
        }
    }
    return SUCCESS;
}


/*
int SetUpSockets(sockaddr_t *tcp_add, sockaddr_t *udp_add,FD conn_fds[FD_SETSIZE])
{
    FD udp_sock = 0;
    FD tcp_sock = 0;
    tcp_sock = SetUpTcpSock(tcp_add);
    if(FAIL == tcp_sock)
    {
        perror("tcp_sock Failed");
    }
    conn_fds[TCP_IDX] = tcp_sock;

    udp_sock = SetUpUdpSock(udp_add);
    if(FAIL == udp_sock)
    {
        close(tcp_sock);
        perror("udp_sock Failed");
        return FAIL;
    }
    conn_fds[UDP_IDX] = udp_sock;
    conn_fds[STDIN_IDX] = STDIN;
}
*/