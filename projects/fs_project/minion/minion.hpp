#ifndef __RD94_MINION_MANAGER_HPP__
#define __RD94_MINION_MANAGER_HPP__


#include <map>

namespace ilrd
{

class Minion
{
public:
    explicit Minion(in_addr_t m_ip, in_port_t m_port);

    

private:
    struct sock_addr_t socket;
    std::map <uint64_t, void *> block_map;
};

}//ilrd

#endif//  __RD94_MINION_MANAGER_HPP__