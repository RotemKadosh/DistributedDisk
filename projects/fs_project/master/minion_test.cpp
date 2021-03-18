#include <iostream>
#include <cstring>
#include "server_udp_socket.hpp"
#include "app_protocol.hpp"
#include "protocol_translator.hpp"
#include <map>
#include <cstddef>// ofsetof

namespace ilrd
{

class Minion
{
public:
    Minion();
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

Minion::Minion():
m_socket(INADDR_ANY, (in_port_t)5002)
{
}

void Minion::Reply()
{
    Request_t req;
    Reply_t rep;
    Read(req); 
    
    if(1 == req.m_type)
    {
        ProcessReadReq(rep, req); 
    }
    else 
    {
        ProcessWriteReq(rep, req);
    }
    Write(rep); 
}
void Minion::Read(Request_t& req)
{

    m_socket.Receive((char *)&req, offsetof(Request_t ,m_block));
    if(req.m_type == 2)
    {
        m_socket.Receive((char *)&(req.m_block), 4096);
    }

}

void Minion::Write(Reply_t& rep)
{
    if(rep.m_type == 2)
    {
        m_socket.Send((char *)&rep, offsetof(Reply_t ,m_block));
    }
    else if(rep.m_type == 1)
    {
        m_socket.Send((char *)&rep, sizeof(Reply_t));
    }
}

void Minion::ProcessReadReq(Reply_t& rep, Request_t& req)
{
   
    if(m_map.find(req.m_offset) != m_map.end())
    {
        
        rep = ProtocolTranslator::TranslateReplay(req.m_id, 1, m_map[req.m_offset], NONE);
    }
    else
    {
        char block[4096] ;
        memset(block, 0, 4096);
        rep = ProtocolTranslator::TranslateReplay(req.m_id, 1, block, BLOCK_NOT_FOUNED); 

    }
}

void Minion::ProcessWriteReq(Reply_t& rep, Request_t& req)
{
    if(m_map.find(req.m_offset) != m_map.end())
    {
        memcpy(m_map[req.m_offset], req.m_block, 4096);
    }
    else
    {
        void *block = NULL;
        try
        {
            block = ::operator new(4096);
        }
        catch(const std::bad_alloc& e)
        {
            rep = ProtocolTranslator::TranslateReplay(req.m_id, 2, NULL, NOT_ENOUGH_SPACE); 
            return;
        }
        memcpy(block, req.m_block, 4096);
        m_map[req.m_offset] = block;
    }
    rep = ProtocolTranslator::TranslateReplay(req.m_id, 2, NULL, NONE); 
}


}//ilrd


int main()
{
    ilrd::Minion minion;
    while (true)
    {
        minion.Reply();
    }
    
    return 0;
}
