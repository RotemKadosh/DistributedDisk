#ifndef __RD94_CLIENT_UDP_SOCKET_HPP__
#define __RD94_CLIENT_UDP_SOCKET_HPP__

#include "socket.hpp"

namespace ilrd
{

class ServerUDPSocket : public Socket
{
public:
    explicit ServerUDPSocket(in_addr_t ip_, in_port_t port_);
    virtual ~ServerUDPSocket();
    virtual bool Send(const void *block_, size_t len);
    virtual bool Receive(char *block_, size_t len);
private:
    struct sockaddr_in m_client_sockaddr;
    socklen_t m_len;
};


}//ilrd

#endif//  __RD94_UDP_SOCKET_HPP__