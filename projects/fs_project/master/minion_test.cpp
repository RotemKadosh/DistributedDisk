#include <iostream>
#include <cstring>
#include "server_udp_socket.hpp"
#include "app_protocol.hpp"
#include "protocol_translator.hpp"
#include <map>

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
m_socket(INADDR_ANY, (in_port_t)5001)
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
        std::cout << "reply:" << std::endl ;
        std::cout << "id: "<< rep.m_id << "\ntype: " << rep.m_type << "\nerror: "<<rep.m_error << "\nblock: " << rep.m_block << std::endl;
    }
    else
    {
        ProcessWriteReq(rep, req);
    }
    Write(rep); 
}
void Minion::Read(Request_t& req)
{
    m_socket.Receive((char *)&req, sizeof(Request_t));
    std::cout << "minion read" << std::endl;
}

void Minion::Write(Reply_t& rep)
{
    m_socket.Send((char *)&rep, sizeof(Reply_t));
    std::cout << "minion write" << std::endl;
}

void Minion::ProcessReadReq(Reply_t& rep, Request_t& req)
{
    std::cout << "minion ProcessReadReq start" << std::endl;
    std::cout << "minion ProcessReadReq key:"<< req.m_offset << std::endl;
    if(m_map.find(req.m_offset) != m_map.end())
    {
        std::cout << "minion ProcessReadReq block found" << std::endl;
        rep = ProtocolTranslator::TranslateReplay(req.m_id, 1, m_map[req.m_offset], NONE);
    }
    else
    {
        std::cout << "minion ProcessReadReq block not found" << std::endl;
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
