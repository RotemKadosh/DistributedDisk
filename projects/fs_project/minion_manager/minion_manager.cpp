#include "minion_manager.hpp"
#include <iostream>

namespace ilrd
{



MinionManager::MinionManager(std::vector<std::pair<in_addr_t, in_port_t> >minions_):
m_num_of_minions(minions_.size())
{
    std::vector<std::pair<in_addr_t, in_port_t> >::iterator iter;
    for(iter = minions_.begin(); iter < minions_.end(); ++iter)
    {
        //std::cout<<"MinionManager in addr: " << iter->first << "\nMinionManager in port: " << iter->second <<"\nMinionManager in htons port: "<<htons(iter->second ) <<std::endl;
        m_minions_data.push_back(MinionData(iter->first, iter->second));
    } 
}

MinionData& MinionManager::GetMinionsData(int minion)
{
    return m_minions_data[minion];
}



}//ilrd








