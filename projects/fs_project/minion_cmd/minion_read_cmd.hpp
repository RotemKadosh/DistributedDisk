#ifndef __RD94_MINION_READ_COMMAND_HPP__
#define __RD94_MINION_READ_COMMAND_HPP__

#include "mediator.hpp"//Command

namespace ilrd
{


class MinReadCmd : public Command
{
public:
    MinReadCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~MinReadCmd() noexcept;
    virtual void Run();
private:
   
};

}//ilrd

#endif//  __RD94_MINION_READ_COMMAND_HPP__