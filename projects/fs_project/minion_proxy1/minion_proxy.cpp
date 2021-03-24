#include "minion_proxy.hpp"
#include "mediator.hpp"
#include <vector> //std::vector
#include <boost/thread/mutex.hpp>//lock_guard
#include "singleton_impl.hpp"
#include "config_manager.hpp"
#include "server_udp_socket.hpp"
#include "app_protocol.hpp"
#include "framework.hpp"
#include <cstdlib>

namespace ilrd
{
MinionProxy::MinionProxy():
m_socket(INADDR_ANY, atoi( GET_CONFIG("PORT_NUM").c_str()))
{
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    boost::shared_ptr<ProxyBase> proxy(this, &Deleter);
    fw->RegisterProxy(m_socket.GetFd(), proxy);
}

MinionProxy::~MinionProxy() noexcept
{
    
}
void MinionProxy::Deleter(MinionProxy *minion)
{
    (void)minion;
}
boost::shared_ptr<CmdData> MinionProxy::ParseData(int sk)
{
    (void)sk;
    Request_t req;
    Read(req);
    return ProcessRequest(req);

}

boost::shared_ptr<CmdData> MinionProxy::ProcessRequest(Request_t& req)
{
    std::vector<char> m_args;
    std::string type;
    if(req.m_type == 1)
    {
        type = "MinReadCmd";
        m_args.resize(sizeof(u_int64_t) * 2);
        memcpy(&(m_args[0]),&req.m_id, sizeof(u_int64_t));
        memcpy(&(m_args[sizeof(u_int64_t)]),&req.m_offset, sizeof(u_int64_t));
    }
    else if(req.m_type ==2)
    {
        type = "MinWriteCmd";
        m_args.resize(sizeof(u_int64_t)*2 + 4096);
        memcpy(&(m_args[0]),&req.m_id, sizeof(u_int64_t));
        memcpy(&(m_args[sizeof(u_int64_t)]),&req.m_offset, sizeof(u_int64_t));
        memcpy(&(m_args[sizeof(u_int64_t)*2]),&req.m_block, 4096);
    }
    return boost::shared_ptr<CmdData>(new CmdData(type, m_args));
}

void MinionProxy::Read(Request_t& req)
{
    m_socket.Receive((char *)&req, offsetof(Request_t ,m_block));
    if(req.m_type == 2)
    {
        m_socket.Receive((char *)&(req.m_block), 4096);
    }
}

void MinionProxy::Reply(Reply_t& rep)
{
    static boost::mutex lock;
    boost::mutex::scoped_lock Lock(lock);
    if(rep.m_type == 2)
    {
        m_socket.Send((char *)&rep, offsetof(Reply_t ,m_block));
    }
    else if(rep.m_type == 1)
    {
        m_socket.Send((char *)&rep, sizeof(Reply_t));
    }
}


}