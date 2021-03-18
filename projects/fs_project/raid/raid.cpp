#include "raid.hpp"


namespace ilrd
{

Raid::Raid(unsigned int num_of_minions):
m_num_of_minions(num_of_minions)
{
    //EMPTY
}

std::pair<int,int> Raid::ChooseMinions(uint64_t offset)
{
    int  minion1 = offset % m_num_of_minions;
    int  minion2 = (minion1 + 1) % m_num_of_minions;
    return std::pair<int, int>(minion1, minion2);
}


}//ilrd

