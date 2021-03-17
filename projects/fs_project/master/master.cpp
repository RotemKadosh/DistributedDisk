#include <iostream>
#include <stdlib.h> // atoi
#include <vector> //std::vector
#include<boost/tokenizer.hpp>
#include <boost/thread/mutex.hpp>//lock_guard
#include "framework.hpp"
#include "master.hpp"
namespace ilrd
{

Request_t Master::TranslateRequest(uint64_t offset, char type, void *block)
{
    return m_translator.TranslateRequest(offset, type, block);
}

std::pair<int,int> Master::ChooseMinions(uint64_t offset)
{
    return m_raid.ChooseMinions(offset);
}

MinionData& Master::GetMinionsData(int minion)
{
    return m_manager.GetMinionsData(minion);
}


boost::shared_ptr<CmdData> Master::ParseData(int sk)
{
    return m_nbd_proxy->ParseData(sk);
}

void Master::InitConnection(std::string nbd_num)
{

    m_nbd_proxy = boost::shared_ptr<NbdProxy>(new NbdProxy(nbd_num.c_str(), ReadBlockSize(), ReadNumOfBlocks()));
    FrameWork *fw = Singleton<FrameWork>::GetInstance();

    boost::shared_ptr<ProxyBase> proxy(this);

    fw->RegisterProxy(m_nbd_proxy->GetFd(), proxy);
    LOG_INFO("before mkfs");

    std::string mkfs("sudo mkfs.ext4 /dev/nbd");
    mkfs += nbd_num;

    system(mkfs.c_str());
    LOG_INFO("after mkfs");
   
    std::cout << "mkfs ends\n";
    std::string dir_path(ReadDirPath());

    std::string mkdir("mkdir ");
    mkdir += dir_path;
    system(mkdir.c_str());

    std::string mount("sudo mount /dev/nbd");
    mount += nbd_num;
    mount += " ";
    mount += dir_path;
    system(mount.c_str());
    LOG_INFO("after mount");

    std::string chmod("sudo chmod 777 ");
    chmod += dir_path;
    system(chmod.c_str());   
}

void Master::Reply(int fd, char* buf, size_t count)
{
    static boost::mutex lock;
    boost::mutex::scoped_lock Lock(lock);
    m_nbd_proxy->NbdReply(fd, buf, count);
}

Master::Master():
m_config_map(GetConfig()),
m_raid(ReadNumOfMinions()),
m_manager(ReadIpsAndPorts()),
m_translator()
{

   
}

Master::~Master() noexcept
{
    //empty
}

boost::shared_ptr<Master::ConfigMap> Master::GetConfig()
{
    // read values into string
    typedef std::istreambuf_iterator<char> str_buf_iter;
    std::ifstream config_fs("./master_configs");
    std::string str((str_buf_iter(config_fs)),str_buf_iter());

    // tokenize string
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> separators(":\n");
    tokenizer tokens(str, separators);
    
    //insert to map
    boost::shared_ptr<ConfigMap> values = boost::make_shared<ConfigMap>(ConfigMap());
    try
    {
        for (tokenizer::iterator tok_iter = tokens.begin(); 
            tok_iter != tokens.end(); ++tok_iter)
        {
            std::string key = *tok_iter;
            (*values)[key] = *(++tok_iter);
            LOG_INFO(key.c_str());
            LOG_INFO((*tok_iter).c_str());
        }
    }
    catch(const std::exception& e)
    {   
        LOG_ERROR("Configuration Fail");
        throw ConfigFailException();
    }

    return values;
}

unsigned int Master::ReadNumOfMinions()
{
    return atoi((*m_config_map)["NUM_OF_MINIONS"].c_str());
}

uint32_t StringToIp(const char *ip)
{
    uint32_t int_ip = 0;
    uint32_t num = 0;
    char *str_num = NULL;
    char buffer[20] ={0};

    assert(NULL != ip);

    strcpy(buffer, ip);

    str_num = strtok(buffer, ".");
    int_ip = atoi(str_num);
    int_ip = int_ip << 24;

    str_num = strtok(NULL, ".");
    num = atoi(str_num);
    int_ip  = int_ip | (num << 26);

    str_num = strtok(NULL, ".");
    num = atoi(str_num);
    int_ip  = int_ip | (num << 8);

    str_num = strtok(NULL, "\0");
    num = atoi(str_num);
    int_ip  = int_ip | num;

    return int_ip;
}


std::vector<std::pair<in_addr_t, in_port_t> > Master::ReadIpsAndPorts()
{
    int num_of_minions = ReadNumOfMinions();
    std::vector<std::pair<in_addr_t, in_port_t> > res;
    std::string min_ip("MINION_IP_");
    std::string min_port("MINION_PORT_");
    for(int i = 2; i <= num_of_minions; ++i)
    {
        char num_of_minion[3];
        sprintf(num_of_minion, "%d", i);
        std::string minion_num(num_of_minion);
        std::string ip_key = min_ip + minion_num;
        std::string port_key = min_port + minion_num;

        res.push_back(std::pair<in_addr_t, in_port_t>(StringToIp((*m_config_map)[ip_key].c_str()), atoi((*m_config_map)[port_key].c_str())));
    }
    return res;
}

const char *Master::ReadDirPath()
{
    return (*m_config_map)["DIR_PATH"].c_str();
}

size_t Master::ReadBlockSize()
{
    return atoi((*m_config_map)["BLOCK_SIZE"].c_str());
}
size_t Master::ReadNumOfBlocks()
{
    return atoi((*m_config_map)["NUM_OF_BLOCKS"].c_str());
}

void Master::DeleteMaster(Master *master)
{
    (void)master;
}

}//ilrd