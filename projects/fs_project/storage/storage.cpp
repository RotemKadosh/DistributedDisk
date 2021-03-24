#include "storage.hpp" 

namespace ilrd
{
    StorageManager::~StorageManager()
    {

    }

    error_t StorageManager::Store(void *block, uint64_t key)
    {
        if(m_map.find(key) == m_map.end())
        {
            void *new_block;
            try
            {
                new_block = ::operator new(4096);
            }
            catch (std::bad_alloc &e)
            {
                return NOT_ENOUGH_SPACE;
            }
            m_map[key] = new_block;
        }
        memcpy(&(m_map[key]), block, 4096);
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