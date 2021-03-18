
#include "write_command.hpp"
#include "nbd_proxy.hpp"//CmdArgs
#include "singleton.hpp"//GetInstance
#include "logger.hpp"//logger
#include "minion_data.hpp"//minion_data
#include "master.hpp"//master
#include "client_udp_socket.hpp"//Clientudpsocket
#include "app_protocol.hpp"//ReReply_t, quser_t
#include "protocol_translator.hpp" //TranslateRequest
#include "framework.hpp" //framework
#include <cstddef> // offsetof

namespace ilrd
{
boost::shared_ptr<Command> CreateWriteCmd(std::vector<char> args)
{
    typedef std::vector<char> data_vec;
    typedef boost::shared_ptr<data_vec> cmd_data_ptr;

    cmd_data_ptr cmd_data = boost::make_shared<data_vec>(data_vec(args));
    return boost::make_shared<WriteCmd>(WriteCmd(cmd_data));
}

void __attribute__((constructor)) Register();

void Register()
{
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    fw->RegisterCmd("WriteCmd", &CreateWriteCmd);
    LOG_INFO("writecommand registered");
}


WriteCmd::WriteCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{
    //REMPTY
}

WriteCmd::~WriteCmd() noexcept
{
    //REMPTY

}

bool WriteCmd::CommunicateMinion(MinionData minion, Request_t& request, Reply_t& reply)
{
    ClientUDPSocket t_socket(minion.GetIp(), minion.GetPort());
    t_socket.Send((char *)&request, offsetof(Request_t, m_block));
    bool status =  t_socket.Receive((char *)&reply, sizeof(Reply_t));

    while (false == status)
    {
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
        {
            t_socket.Send((char *)&request, offsetof(Request_t, m_block));
        }
        else
        {
            break;
        } 
        status =  t_socket.Receive((char *)&reply, sizeof(Reply_t));
    }
    return status;
} 
void WriteCmd::Run()
{    
    NbdProxy::CmdArgs args;
    memcpy(&args, &(*m_data)[0], sizeof(args));

    Master *master = Singleton<Master>::GetInstance();
    std::pair<int,int> minions = master->ChooseMinions(args.key);
    MinionData min1 = master->GetMinionsData(minions.first);
    MinionData min2 = master->GetMinionsData(minions.second);

    Request_t req = ProtocolTranslator::TranslateRequest(args.key, 2, args.block);
    Reply_t rep1;
    Reply_t rep2;
    
    CommunicateMinion(min1, req, rep1);
    CommunicateMinion(min2, req, rep2);
    if(rep1.m_error != NONE || rep2.m_error != NONE)
    {
        LOG_ERROR("error in writing block");
    }
    master->Reply((int)args.fd,(char *) &args.reply, sizeof(struct nbd_reply));
}


}

int main()
{

    return 0;
}
