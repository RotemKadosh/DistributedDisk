#ifndef __RD94_WRITE_COMMAND_HPP__
#define __RD94_WRITE_COMMAND_HPP__

#include "mediator.hpp"//Command

namespace ilrd
{


class WriteCmd : public Command
{
public:
    WriteCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~WriteCmd() noexcept;
    virtual void Run();
};

}//ilrd

#endif//  __RD94_WRITE_COMMAND_HPP__