#ifndef __RD94_MINION_MANAGER_HPP__
#define __RD94_MINION_MANAGER_HPP__

#include <vector>// vector
#include <netinet/in.h>//in_addr_t, in_port_t
#include <utility> // pair
#include "minion_data.hpp"//MinionData


namespace ilrd
{

class MinionManager
{
public:
    explicit MinionManager(std::vector<std::pair<in_addr_t, in_port_t> >minions_);
    MinionData& GetMinionsData(int minion);

private:

    const unsigned int m_num_of_minions;
    std::vector<MinionData> m_minions_data;
};

}//ilrd

#endif//  __RD94_MINION_MANAGER_HPP__