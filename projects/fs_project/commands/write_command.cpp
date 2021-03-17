
#include "write_command.hpp"
#include "nbd_proxy.hpp"//CmdArgs
#include "singleton.hpp"//GetInstance
#include "logger.hpp"//logger
#include "minion_data.hpp"//minion_data
#include "master.hpp"//master
#include "client_udp_socket.hpp"//Clientudpsocket
#include "app_protocol.hpp"//Requser_t, Reply_t
#include "protocol_translator.hpp" //TranslateRequest
#include "framework.hpp" //framework

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

void WriteCmd::Run()
{    
    NbdProxy::CmdArgs args;
    LOG_INFO("writecommand run");
    memcpy(&args, &(*m_data)[0], sizeof(args));

    Master *master = Singleton<Master>::GetInstance();
    std::pair<int,int> minions = master->ChooseMinions(args.key);

    MinionData min1 = master->GetMinionsData(minions.first);

    Request_t req = ProtocolTranslator::TranslateRequest(args.key, 2, args.block);
    ClientUDPSocket socket1(min1.GetIp(), min1.GetPort());
    socket1.Send((char *)&req, sizeof(Request_t));
    LOG_INFO("writecommand send");
    Reply_t rep1;
    socket1.Receive((char *)&rep1, sizeof(Reply_t));
    if(rep1.m_error != NONE)
    {
        LOG_ERROR("error in writing block");
    }
    LOG_INFO("writecommand Receive");
    MinionData min2 = master->GetMinionsData(minions.second);
    ClientUDPSocket socket2(min2.GetIp(), min2.GetPort());
    socket2.Send((char *)&req, sizeof(Request_t));
    LOG_INFO("writecommand send2");
    Reply_t rep2;
    socket2.Receive((char *)&rep2, sizeof(Reply_t));
    if(rep2.m_error != NONE)
    {
        LOG_ERROR("error in writing block");
    }
    LOG_INFO("writecommand Receive2");
    
    
    master->Reply((int)args.fd,(char *) &args.reply, sizeof(struct nbd_reply));
}



    
}

int main()
{

    return 0;
}
