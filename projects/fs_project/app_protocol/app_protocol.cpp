#include "app_protocol.hpp"

namespace ilrd
{

    Request::Request()
    {

    }

    Request::Request(uint64_t id_, unsigned char type_, uint64_t offset_, const void *block_):
    m_id(id_),
    m_type(type_),
    m_offset(offset_),
    m_block()
    {
        if(NULL != block_)
        {
            memcpy(&m_block, block_, 4096);
        }
         
    }

    Reply::Reply()
    {
        
    }
    Reply::Reply(uint64_t id_, unsigned char type_, error_t error_, const void *block_):
    m_id(id_),
    m_type(type_),
    m_error(error_),
    m_block()
    {
        if(NULL != block_)
        {
            memcpy(&m_block, block_, 4096);
        }
           
    }

}