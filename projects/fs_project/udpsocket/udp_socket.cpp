#include "udp_socket.hpp"
#include "logger.hpp" // Logger

namespace ilrd
{

UDPSocket::UDPSocket(in_addr_t ip_, in_port_t port_):
Socket(SOCK_DGRAM, ip_, port_)
{
    //  EMPTY   
}

UDPSocket::~UDPSocket()
{
    //  EMPTY  
}

bool UDPSocket::Send(const void *block_, size_t len)
{
    if(-1 == sendto(m_sockfd, block_, len, 0, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)))
    {
        LOG_ERROR("sendto failed");
        return false; 
    }
}

bool UDPSocket::Receive(char *block_, size_t len)
{
    if(-1 == recvfrom(m_sockfd, block_, len, 0, (struct sockaddr*)NULL, NULL))
    {
        LOG_ERROR("recvfrom failed");
        return false; 
    }
}


}//ilrd