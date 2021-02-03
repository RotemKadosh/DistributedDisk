#include <sys/select.h>/* select fd_set, FD_ISSET, FD_ZERO, FD_SETSIZE*/
#include <stdio.h> /*printf, perror*/
#include <string.h> /*strcnp,*/
#include <sys/socket.h>/*bind, sendto, recvfrom, accept*/
#include <unistd.h> /*read, write*/
#include <fcntl.h> /*open*/
#include <assert.h> /*assert*/
#include <boost/bind.hpp>    // function
#include <vector> //std::vector
#include "../../system_programming/ping_pong_server/udp_tcp.h"
#include "reactor.hpp"
#include <boost/thread.hpp> //thread

#define UDP_PORT (5010)
#define TCP_PORT (5011)
#define FAIL (-1)
#define SUCCESS (0)
#define REQUEST_Q_SZ (5)
#define MSGSZ (10)
#define RUN (0)
#define STOP (1)
#define UDP_IDX (1)
#define TCP_IDX (0)
#define STOP (1)
#define NULL_BYTE (1)
#define EMPTY (-1)
#define DISCONNECT (1)
#define MAX(a,b) ((a) > (b) ? (a) : (b))

using namespace ilrd;

class Server
{
    
public:
    explicit Server();
    ~Server();
    void Run(); // blocking
    void Stop();

    struct ServerException : public std::runtime_error
    {
        ServerException(): std::runtime_error(""){}
        const char * what () const throw ()
        {
            return "Server Fail";
        }
    };

private:

    Reactor m_reactor;
    std::vector<int> m_fds;

  
    int ComTcpCli(int fd);
    int ComUdpCli(int udp_fd);
    int AcceptTcpCli(int tcp_fd);
};

Server::~Server()
{
    for(std::vector<int>::iterator it = m_fds.begin(); it != m_fds.end(); ++it)
    {
        close(*it);
    }
}
void Server::Stop()
{
    m_reactor.Stop();
}

void Server::Run()
{
    m_reactor.Run();
}

int CreateTCPsocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int CreateUDPsocket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

void InitSocketAddr(sockaddr_t *sockaddr, in_addr_t s_addr, in_port_t port)
{
    memset(sockaddr, 0, sizeof(*sockaddr));
    sockaddr->sin_addr.s_addr = htonl(s_addr); 
    sockaddr->sin_port = htons(port); 
    sockaddr->sin_family = AF_INET; 
}

int CreateAndSetUpUDPSocket(sockaddr_t *servaddr)
{
    int udp_fd = CreateUDPsocket();
    if(FAIL == udp_fd)
    {
        perror("socket failed: ");
        return FAIL;
    }

    InitSocketAddr(servaddr, INADDR_ANY, UDP_PORT);

    if(FAIL == bind(udp_fd, (struct sockaddr*)servaddr, sizeof(*servaddr)))
    {
        perror("bind failed: ");
        close(udp_fd);
        return FAIL;        
    }

    return udp_fd;
}

static int CreateAndSetUpTCPSocket(sockaddr_t *servaddr)
{
    int tcp_fd = CreateTCPsocket(); 
    if(FAIL == tcp_fd)
    {
        perror("socket failed: ");
        return FAIL;
    }

    InitSocketAddr(servaddr, INADDR_ANY, TCP_PORT);
  
    if(FAIL == bind(tcp_fd, (struct sockaddr *)servaddr, sizeof(*servaddr)))
    {
        perror("bind failed: ");
        close(tcp_fd);
        return FAIL;        
    } 

    if (FAIL == listen(tcp_fd, REQUEST_Q_SZ))
    { 
        perror("listen failed: ");
        close(tcp_fd);
        return FAIL; 
    }

    return tcp_fd;
}
bool CreateAndSetUpSocket(sockaddr_t *tcp_addr, sockaddr_t *udp_addr, int *tcp_fd, int *udp_fd)
{

    *tcp_fd = CreateAndSetUpTCPSocket(tcp_addr);
    if(FAIL == *tcp_fd)
    {
        return false;
    }
    *udp_fd = CreateAndSetUpUDPSocket(udp_addr);
    if(FAIL == *udp_fd)
    {
        close(*tcp_fd);
        return false;
    }
    return true;
}

Server::Server()
{
    sockaddr_t udp_addr, tcp_addr;
    int udp_fd = 0, tcp_fd = 0;

    if(false == CreateAndSetUpSocket(&tcp_addr, &udp_addr, &tcp_fd, &udp_fd))
    {
        throw ServerException();
    }
    m_reactor.Add(udp_fd, Reactor::READ, boost::bind(&Server::ComUdpCli, this ,udp_fd));
    m_reactor.Add(tcp_fd, Reactor::READ, boost::bind(&Server::AcceptTcpCli, this ,tcp_fd));
    m_fds.push_back(udp_fd);
    m_fds.push_back(tcp_fd);
    

}

int Server::ComUdpCli(int udp_fd)
{
    char buffer[MSGSZ] = {0};
    int rec_len = 0;
    sockaddr_t cliaddr;
    char message[] = "pong";
    socklen_t len = sizeof(struct sockaddr);    

    rec_len = recvfrom(udp_fd, buffer, sizeof(buffer) - NULL_BYTE , 0, (struct sockaddr*)&cliaddr, &len);
    if(FAIL == rec_len)
    {
        perror("recvfrom failed: ");
        return FAIL; 
    }
    buffer[rec_len] = '\0';
    printf("received from udp client %s\n", buffer);
    if(FAIL == sendto(udp_fd, message, sizeof(message), 0, (struct sockaddr*)&cliaddr, len))
    {
        perror("sendto failed: ");
        return FAIL;        
    }

    return SUCCESS; 
}

int Server::AcceptTcpCli(int tcp_fd)
{
    int conn_fd = accept(tcp_fd, (struct sockaddr *)NULL, NULL); 
    if (FAIL == conn_fd) 
    { 
        return FAIL;
    }
    m_reactor.Add(conn_fd, Reactor::READ, boost::bind(&Server::ComTcpCli,this, conn_fd));
    m_fds.push_back(conn_fd);
    return SUCCESS;

}

int Server::ComTcpCli(int fd)
{
    char buff[10] = {0};
    int read_len = 0;

    memset(buff, 0, sizeof(buff));
    read_len = read(fd, buff, sizeof(buff) - NULL_BYTE);
    buff[read_len] = '\0';
    if(0 < read_len)
    {
       printf("received from fd%d %s\n", fd, buff);
    }
    else if(0 == read_len)
    {
        close(fd);
        return DISCONNECT; 
    }
    else
    {
        perror("read failed: ");
        return FAIL; 
    }
    char message[] = "pong";
    if(FAIL == write(fd, message, sizeof(message)))
    {
        perror("write failed: ");
        return FAIL; 
    }
      
    return SUCCESS;
}

void stopF2(Server *r)
{
    sleep(15);

    r->Stop();
}


int main()
{
    Server server;
    boost::thread th(boost::bind(stopF2, &server));
    server.Run();
    return 0;
}
