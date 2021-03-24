#ifndef __RD94_MINION_WRITE_COMMAND_HPP__
#define __RD94_MINION_WRITE_COMMAND_HPP__

#include "mediator.hpp"//Command

namespace ilrd
{


class MinWriteCmd : public Command
{
public:
    MinWriteCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~MinWriteCmd() noexcept;
    virtual void Run();
private:
   
};

}//ilrd

#endif//  __RD94_MINION_WRITE_COMMAND_HPP__