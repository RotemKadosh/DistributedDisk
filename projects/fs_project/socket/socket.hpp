#ifndef __RD94_SOCKET_HPP__
#define __RD94_SOCKET_HPP__

#include <netinet/in.h>//in_addr_t, in_port_t, sockaddr_in



namespace ilrd
{

class Socket
{
public:
    explicit Socket(enum __socket_type sock_type_, in_addr_t ip_, in_port_t port_);
    virtual ~Socket();
    virtual bool Send(const void *block_, size_t len) =0;
    virtual bool Receive(char *block_, size_t len) =0;
    void Bind();
protected:
    int m_sockfd;
    struct sockaddr_in m_sockaddr;
};

}//ilrd

#endif//  __RD94_SOCKET_HPP__