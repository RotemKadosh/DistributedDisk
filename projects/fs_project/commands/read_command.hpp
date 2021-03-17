#ifndef __RD94_READ_COMMAND_HPP__
#define __RD94_READ_COMMAND_HPP__

#include "mediator.hpp"//Command

namespace ilrd
{


class ReadCmd : public Command
{
public:
    ReadCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~ReadCmd() noexcept;
    virtual void Run();
};

}//ilrd

#endif//  __RD94_READ_COMMAND_HPP__