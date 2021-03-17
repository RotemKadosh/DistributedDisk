#ifndef __RD94_MINION_DATA_HPP__
#define __RD94_MINION_DATA_HPP__


#include <netinet/in.h>//in_addr_t, in_port_t

namespace ilrd
{

class MinionData
{
public:
    explicit MinionData(in_addr_t ip_, in_port_t port_);
    unsigned int GetIp();
    unsigned int GetPort();
private:
    in_addr_t m_ip;
    in_port_t m_port;
};

}//ilrd



#endif//  __RD94_MINION_DATA_HPP__