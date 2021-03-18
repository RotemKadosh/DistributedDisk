#ifndef __RD94_MINION_HPP__
#define __RD94_MINION_HPP__

#include "server_udp_socket.hpp"
#include "app_protocol.hpp"
#include "protocol_translator.hpp"
#include <map>


namespace ilrd
{

class Minion
{
public:
    explicit Minion();
    //~Minion();
    void Reply();

private:

    void Read(Request_t& req);
    void Write(Reply_t& rep);
    void ProcessReadReq(Reply_t& rep, Request_t& req);
    void ProcessWriteReq(Reply_t& rep, Request_t& req);
    ServerUDPSocket m_socket;
    std::map<uint64_t, void *> m_map;
};


}//ilrd

#endif// __RD94_MINION_HPP__