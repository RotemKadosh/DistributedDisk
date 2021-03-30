#include <string.h> //*memset
#include <sys/socket.h>/*socket*/
#include "socket.hpp"
#include "logger.hpp" // Logger
#include <iostream>

namespace ilrd
{

Socket::Socket(enum __socket_type sock_type_, in_addr_t ip_, in_port_t port_):
m_sockfd(socket(AF_INET, sock_type_, 0)),
m_sockaddr()
{
    //LOG_INFO("Socket ctor");
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    m_sockaddr.sin_addr.s_addr = htonl(ip_); 
    m_sockaddr.sin_port = htons(port_); 
    m_sockaddr.sin_family = AF_INET;

 
}

Socket::~Socket()
{
    if(-1 == close(m_sockfd))
    {
        LOG_ERROR("close socket failed");
    }
}

void Socket::Bind()
{
    //LOG_INFO("before bind");
    if(-1 == bind(m_sockfd, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)))
    {
        perror("bind fail:");
        LOG_ERROR("bind failed: ");      
        throw ;
        //ADD EXCEPTION       
    }
    //LOG_INFO("bind");
}



}//ilrd

