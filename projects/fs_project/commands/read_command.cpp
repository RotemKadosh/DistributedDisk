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
#include <cstddef> // offsetof
#include <iostream>
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
    //LOG_INFO("ReadCmd::Register() start");
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    //LOG_INFO("fw");
    fw->RegisterCmd("ReadCmd", &CreateReadCmd);
    //LOG_INFO("fw registered");
    //LOG_INFO("readcommand registered");
}


ReadCmd::ReadCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{
   //LOG_INFO("readcommand create");
}

ReadCmd::~ReadCmd() noexcept
{
    //EMPTY
}

void ReadCmd::Run()
{

    NbdProxy::CmdArgs args;
    memcpy(&args, &(*m_data)[0], sizeof(args));

    Master *master = Singleton<Master>::GetInstance();
    std::pair<int,int> minions = master->ChooseMinions(args.key);
    MinionData min1 = master->GetMinionsData(minions.first);

    Request_t req = ProtocolTranslator::TranslateRequest(args.key, 1, NULL);
    Reply_t rep;

    if(true != CommunicateMinion(min1, req, rep))
    {
        MinionData min2 = master->GetMinionsData(minions.second);
        if(false == CommunicateMinion(min2, req, rep))
        {
            //LOG_ERROR("readcommand:: recvfrom failed");
            return;
        }

    }
    master->Reply((int)args.fd,(char *) &args.reply, sizeof(struct nbd_reply));
    master->Reply(args.fd, rep.m_block, 4096);

}

bool ReadCmd::CommunicateMinion(MinionData minion, Request_t& request, Reply_t& reply)
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

}//ilrd

int main()
{

    return 0;
}