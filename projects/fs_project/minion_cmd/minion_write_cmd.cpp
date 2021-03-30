#include "minion_write_cmd.hpp"
#include "framework.hpp" //fw
#include "app_protocol.hpp"
#include "minion_proxy.hpp"
#include "protocol_translator.hpp"
#include "storage.hpp"


namespace ilrd
{

MinWriteCmd::MinWriteCmd(boost::shared_ptr<std::vector<char> > data_):
Command(data_)
{

}
MinWriteCmd::~MinWriteCmd() noexcept
{

}
boost::shared_ptr<Command> CreateMinWriteCmd(std::vector<char> args)
{
    typedef std::vector<char> data_vec;
    typedef boost::shared_ptr<data_vec> cmd_data_ptr;

    cmd_data_ptr cmd_data = boost::make_shared<data_vec>(data_vec(args));
    return boost::make_shared<MinWriteCmd>(MinWriteCmd(cmd_data));
}

void __attribute__((constructor)) MinWRegister();

void MinWRegister()
{
    //LOG_INFO("WriteCmd::Register() start");
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    
    fw->RegisterCmd("MinWriteCmd", &CreateMinWriteCmd);
   // LOG_INFO("fw registered");
    //LOG_INFO("readcommand registered");
}

void MinWriteCmd::Run()
{
    StorageManager *storage = Singleton<StorageManager>::GetInstance();

    uint64_t offset;
    uint64_t id;
    char block[4096]; 
    memcpy(&id, &(*m_data)[0], sizeof(uint64_t));
    memcpy(&offset, &(*m_data)[sizeof(uint64_t)], sizeof(uint64_t));

    error_t error = storage->Store(&(*m_data)[sizeof(uint64_t) * 2], offset);

    MinionProxy *minion_proxy = Singleton<MinionProxy>::GetInstance();
    Reply_t rep = ProtocolTranslator::TranslateReply(id,2, block, error);
    minion_proxy->Reply(rep);

}


}//ilrd

int main()
{
    return 0;
}