#include <iostream>
#include <stdlib.h> // atoi
#include <vector> //std::vector
#include<boost/tokenizer.hpp>
#include <boost/thread/mutex.hpp>//lock_guard
#include "framework.hpp"
#include "master.hpp"
#include "config_manager.hpp"
#include "singleton_impl.hpp"
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
    //LOG_INFO("before mkfs");

    std::string mkfs("sudo mkfs.ext4 /dev/nbd");
    mkfs += nbd_num;

    system(mkfs.c_str());
    //("after mkfs");
   
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
    //LOG_INFO("after mount");

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
m_raid(ReadNumOfMinions()),
m_manager(ReadIpsAndPorts()),
m_translator()
{

   
}

Master::~Master() noexcept
{
    //empty
}

unsigned int Master::ReadNumOfMinions()
{
    return atoi(GET_CONFIG("NUM_OF_MINIONS").c_str());
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
    for(int i = 1; i <= num_of_minions; ++i)
    {
        char num_of_minion[3];
        sprintf(num_of_minion, "%d", i);
        std::string minion_num(num_of_minion);
        std::string ip_key = min_ip + minion_num;
        std::string port_key = min_port + minion_num;
        //std::cout<<"ip: " << StringToIp((*m_config_map)[ip_key].c_str()) << std::endl;
        //std::cout<<"port: " << atoi((*m_config_map)[port_key].c_str()) << std::endl;
        res.push_back(std::pair<in_addr_t, in_port_t>(StringToIp(GET_CONFIG(ip_key).c_str()), atoi(GET_CONFIG(port_key).c_str())));
        //std::cout<<"in addr: " << res.back().first << "\nin port: " << res.back().second << std::endl;
    
    }
    return res;
}

const char *Master::ReadDirPath()
{
    return GET_CONFIG("DIR_PATH").c_str();
}

size_t Master::ReadBlockSize()
{
    return atoi(GET_CONFIG("BLOCK_SIZE").c_str());
}
size_t Master::ReadNumOfBlocks()
{

     return atoi(GET_CONFIG("NUM_OF_BLOCKS").c_str());
}

void Master::DeleteMaster(Master *master)
{
    (void)master;
}

}//ilrd