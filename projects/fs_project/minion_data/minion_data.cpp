#include "minion_data.hpp"


namespace ilrd
{

    MinionData::MinionData(in_addr_t ip_, in_port_t port_):
    m_ip(ip_),
    m_port(port_)
    {
        //empty
    }

    unsigned int MinionData::GetIp()
    {
        return m_ip;
    }

    unsigned int MinionData::GetPort()
    {
        return m_port;
    }

}//ilrd

