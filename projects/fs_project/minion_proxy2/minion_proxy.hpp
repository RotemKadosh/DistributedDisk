#ifndef __RD94_MINION_HPP__
#define __RD94_MINION_HPP__

#include "server_udp_socket.hpp"
#include "app_protocol.hpp"
#include "mediator.hpp"
#include "singleton.hpp"

namespace ilrd
{

class MinionProxy: private boost::noncopyable, public ProxyBase
{
public:
    boost::shared_ptr<CmdData> ParseData(int sk);
    void Reply(Reply_t& rep);
    static void Deleter(MinionProxy *);
private:
    friend class Singleton<MinionProxy>;
    explicit MinionProxy();
    virtual ~MinionProxy() noexcept;

    void Read(Request_t& req);

    boost::shared_ptr<CmdData> ProcessRequest(Request_t& req);
    ServerUDPSocket m_socket;  
};



}//ilrd

#endif// __RD94_MINION_HPP__