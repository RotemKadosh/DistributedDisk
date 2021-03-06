#ifndef __RD94_MASTER_HPP__
#define __RD94_MASTER_HPP__

#include <utility> // pair
#include <boost/noncopyable.hpp>//boost::noncopyable
#include <boost/unordered_map.hpp> //boost::unordered_map
#include <boost/shared_ptr.hpp> //boost::shared_ptr
#include "app_protocol.hpp"//Request_t
#include "mediator.hpp"//ProxyBase
#include "nbd_proxy.hpp" //NbdProxy
#include "singleton.hpp" //singleton<Master>
#include "raid.hpp"//Raid
#include "minion_data.hpp"//MinionData
#include "protocol_translator.hpp" //ProtocolTranslator
#include "minion_manager.hpp" //MinionManager
#include "logger.hpp" // logger
#include "utils.hpp" // logger
namespace ilrd
{

class Master: private boost::noncopyable, public ProxyBase
{
public:

    Request_t TranslateRequest(uint64_t offset, char type, void *block);
    std::pair<int,int> ChooseMinions(uint64_t offset);
    MinionData& GetMinionsData(int minion);
    boost::shared_ptr<CmdData> ParseData(int sk);
    void Reply(int fd, char *buf, size_t count);


    void InitConnection(std::string nbd_num);
private:
    static void DeleteMaster(Master *master);
    virtual ~Master() noexcept;
    friend class Singleton<Master>;
    explicit Master();
    unsigned int ReadNumOfMinions();
    std::vector<std::pair<in_addr_t, in_port_t> > ReadIpsAndPorts();
    const char *ReadDirPath();
    size_t ReadBlockSize();
    size_t ReadNumOfBlocks();

    Raid m_raid;
    MinionManager m_manager;
    ProtocolTranslator m_translator;
    boost::shared_ptr<NbdProxy> m_nbd_proxy;


};




}//ilrd

#endif//  __RD94_MASTER_HPP__