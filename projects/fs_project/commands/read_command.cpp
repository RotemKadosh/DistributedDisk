#include "read_command.hpp"
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

boost::shared_ptr<Command> CreateReadCmd(std::vector<char> args)
{
    typedef std::vector<char> data_vec;
    typedef boost::shared_ptr<data_vec> cmd_data_ptr;

    cmd_data_ptr cmd_data = boost::make_shared<data_vec>(data_vec(args));
    return boost::make_shared<ReadCmd>(ReadCmd(cmd_data));
}

void __attribute__((constructor)) Register();

void Register()
{
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    LOG_INFO("fw");
    fw->RegisterCmd("ReadCmd", &CreateReadCmd);
    LOG_INFO("fw registered");
    LOG_INFO("readcommand registered");
}


ReadCmd::ReadCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{
   LOG_INFO("readcommand create");
}

ReadCmd::~ReadCmd() noexcept
{
    //EMPTY
}

void ReadCmd::Run()
{
    LOG_INFO("ReadCmd::Run() start");
    NbdProxy::CmdArgs args;

    memcpy(&args, &(*m_data)[0], sizeof(args));

    FrameWork * fw = Singleton<FrameWork>::GetInstance();


    Master *master = Singleton<Master>::GetInstance();

    std::pair<int,int> minions = master->ChooseMinions(args.key);

    MinionData min1 = master->GetMinionsData(minions.first);

    Request_t req = ProtocolTranslator::TranslateRequest(args.key, 1, NULL);

    ClientUDPSocket t_socket(min1.GetIp(), min1.GetPort());
    Reply_t rep;

    t_socket.Send((char *)&req, sizeof(Request_t));
    bool status =  t_socket.Receive((char *)&rep, sizeof(Reply_t));
    while (false == status)
    {
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
        {
            LOG_INFO("readcommand:: recvfrom timeout");
            t_socket.Send((char *)&req, sizeof(Request_t));
        }
        else
        {
            break;
        } 
        status =  t_socket.Receive((char *)&rep, sizeof(Reply_t));
    }
    if(true != status)
    {
        MinionData min2 = master->GetMinionsData(minions.second);
        ClientUDPSocket t_socket2(min2.GetIp(), min2.GetPort());
        t_socket2.Send((char *)&req, sizeof(Request_t));
        status =  t_socket2.Receive((char *)&rep, sizeof(Reply_t));
        while (false == status)
        {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                LOG_INFO("readcommand:: recvfrom timeout");
                t_socket2.Send((char *)&req, sizeof(Request_t));
            }
            else
            {
                break;
            } 
            status =  t_socket2.Receive((char *)&rep, sizeof(Reply_t));
        }
        if(false == status)
        {
            LOG_ERROR("readcommand:: recvfrom failed");
            return;
        }

    }

    
    
    LOG_INFO("readcommand Receive");

    master->Reply((int)args.fd,(char *) &args.reply, sizeof(struct nbd_reply));
    master->Reply(args.fd, rep.m_block, 4096);

}



}//ilrd

int main()
{

    return 0;
}