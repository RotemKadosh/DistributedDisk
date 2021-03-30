#include "storage.hpp" 
#include "logger.hpp"

namespace ilrd
{

    StorageManager::StorageManager():
    m_map()
    {

    }
    StorageManager::~StorageManager()
    {

    }

    error_t StorageManager::Store(void *block, uint64_t key)
    {
        if(m_map.find(key) != m_map.end())
        {
            delete(m_map[key]);
        }
  
        void *new_block = ::operator new(4096);
  
        
        memcpy(new_block, block, 4096);
        m_map[key] = new_block;

        return NONE;
    }

    error_t StorageManager::Load(void *block,uint64_t key)
    {
        if(m_map.find(key) == m_map.end())
        {
            memset(block, 0, 1);
            return BLOCK_NOT_FOUNED;
        }
        memcpy(block, &(m_map[key]),4096);
        return NONE;
    }
}