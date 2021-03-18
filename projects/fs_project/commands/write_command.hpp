#ifndef __RD94_WRITE_COMMAND_HPP__
#define __RD94_WRITE_COMMAND_HPP__

#include "mediator.hpp"//Command
#include "minion_data.hpp" 
#include "app_protocol.hpp"

namespace ilrd
{


class WriteCmd : public Command
{
public:
    WriteCmd(boost::shared_ptr<std::vector<char> > data_);
    virtual ~WriteCmd() noexcept;
    virtual void Run();
private:
    bool CommunicateMinion(MinionData minion, Request_t& request, Reply_t& reply);

};

}//ilrd

#endif//  __RD94_WRITE_COMMAND_HPP__