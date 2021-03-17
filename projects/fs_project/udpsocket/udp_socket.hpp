#ifndef __RD94_UDP_SOCKET_HPP__
#define __RD94_UDP_SOCKET_HPP__

#include "socket.hpp"

namespace ilrd
{

class UDPSocket : public Socket
{
public:
    explicit UDPSocket(in_addr_t ip_, in_port_t port_);
    virtual ~UDPSocket();
    virtual bool Send(const void *block_, size_t len);
    virtual bool Receive(char *block_, size_t len);

};


}//ilrd

#endif//  __RD94_UDP_SOCKET_HPP__