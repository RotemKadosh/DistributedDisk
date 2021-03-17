#ifndef __RD94_CLIENT_UDP_SOCKET_HPP__
#define __RD94_CLIENT_UDP_SOCKET_HPP__

#include "socket.hpp"

namespace ilrd
{

class ClientUDPSocket : public Socket
{
public:
    explicit ClientUDPSocket(in_addr_t ip_, in_port_t port_);
    virtual ~ClientUDPSocket();
    virtual bool Send(const void *block_, size_t len);
    virtual bool Receive(char *block_, size_t len);

};


}//ilrd

#endif//  __RD94_UDP_SOCKET_HPP__