#ifndef __RD94_STORAGE_HPP__
#define __RD94_STORAGE_HPP__


#include <map>
#include "app_protocol.hpp"
#include "singleton.hpp"

namespace ilrd
{

class StorageManagerBase
{
public:
    virtual error_t Store(void *block, uint64_t key) =0;
    virtual error_t Load(void *block,uint64_t key) =0;
    explicit StorageManagerBase(){};
    virtual ~StorageManagerBase(){};
};

class StorageManager: public StorageManagerBase
{
public:
    virtual error_t Store(void *block, uint64_t key);
    virtual error_t Load(void *block,uint64_t key);
    virtual ~StorageManager();
private:
    friend class Singleton<StorageManager>;
    std::map<uint64_t, void *> m_map;
    explicit StorageManager();
    
};
}



#endif 