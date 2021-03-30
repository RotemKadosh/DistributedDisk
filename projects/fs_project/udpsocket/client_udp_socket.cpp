#include "client_udp_socket.hpp"
#include "logger.hpp" // Logger
#include <iostream>

namespace ilrd
{

ClientUDPSocket::ClientUDPSocket(in_addr_t ip_, in_port_t port_):
Socket(SOCK_DGRAM, INADDR_LOOPBACK, port_)
{
    //LOG_INFO("ClientUDPSocket ctor");
  
}

ClientUDPSocket::~ClientUDPSocket()
{
    //  EMPTY  
}

bool ClientUDPSocket::Send(const void *block_, size_t len)
{

   // LOG_INFO("ClientUDPSocket::Send start");
   // std::cout << "clientUDPSocket:: send fd: "<< m_sockfd<< std::endl;
   // std::cout << "clientUDPSocket:: send m_sockaddr.port: "<< m_sockaddr.sin_port<< std::endl;
   // std::cout << "clientUDPSocket:: send m_sockaddr.s_add: "<< m_sockaddr.sin_addr.s_addr<< std::endl;

    if(-1 == sendto(m_sockfd, block_, len, 0, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)))
    {
        //perror("ClientUDPSocket:sendto: ");
        //LOG_ERROR("Socket:sendto failed");
        return false; 
    }
    //LOG_INFO("ClientUDPSocket::sendto finish");
    return true;
}

bool ClientUDPSocket::Receive(char *block_, size_t len)
{
    //LOG_INFO("ClientUDPSocket::Receive start");
    struct timeval tv;
    tv.tv_sec = 4;
    tv.tv_usec = 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) 
    {
        LOG_ERROR("ClientUDPSocket :: setsockopt Error");
        perror("setsockopt Error");
    } 

    if(-1 == recvfrom(m_sockfd, block_, len, 0, (struct sockaddr*)NULL, NULL))
    {
        //LOG_ERROR("Socket:recvfrom failed");
        //perror("ClientUDPSocket:recvfrom: ");
        return false; 
    }
    //LOG_INFO("ClientUDPSocket::Receive finish");
    return true;
}


}//ilrd