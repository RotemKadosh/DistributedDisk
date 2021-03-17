#ifndef __RD94_RAID_HPP__
#define __RD94_RAID_HPP__

#include <utility> // pair
#include <stdint.h> // unit64_t


namespace ilrd
{

class Raid
{
public:
    explicit Raid(unsigned int num_of_minions);
    std::pair<int,int> ChooseMinions(uint64_t offset);

private:
    unsigned int m_num_of_minions;
};

}//ilrd

#endif//  __RD94_RAID_HPP__