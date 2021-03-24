#include "server_udp_socket.hpp"
#include "logger.hpp" // Logger
#include <iostream>

namespace ilrd
{

ServerUDPSocket::ServerUDPSocket(in_addr_t ip_, in_port_t port_):
Socket(SOCK_DGRAM, ip_, port_),
m_len(sizeof(struct sockaddr))
{
    LOG_INFO("ClientUDPSocket ctor"); 
    Bind();
}

ServerUDPSocket::~ServerUDPSocket()
{
    //  EMPTY  
}

int ServerUDPSocket::GetFd()
{
    return this->m_sockfd;
}

bool ServerUDPSocket::Send(const void *block_, size_t len)
{
    std::cout<<"Send:len: " << len << std::endl;
    if(-1 == sendto(m_sockfd, block_, len, 0, (struct sockaddr*)&m_client_sockaddr, m_len))
    {
        LOG_ERROR("Socket:sendto failed");
        return false; 
    }
    //std::cout<<"len: " << len << std::endl;
    //std::cout <<"m_len: " << m_len << std::endl;;
    LOG_INFO("sendto");
    return true;
}

bool ServerUDPSocket::Receive(char *block_, size_t len)
{
    if(-1 == recvfrom(m_sockfd, block_, len, 0, (struct sockaddr*)&m_client_sockaddr, &m_len))
    {
        perror("Socket:recvfrom");
        LOG_ERROR("Socket:recvfrom failed");
        return false; 
    }
    std::cout<<"Receive:len: " << len << std::endl;
    std::cout <<"Receive:m_len: " << m_len << std::endl;;
    LOG_INFO("recvfrom");
    return true;
}


}//ilrd