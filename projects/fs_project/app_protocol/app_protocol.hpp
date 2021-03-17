#ifndef __RD94_PROTOCOL_HPP__
#define __RD94_PROTOCOL_HPP__

#include <stdint.h> // uint64_t 
#include <cstring> //memcpy

namespace ilrd
{

typedef enum error
{
    NONE = 0,
    NOT_ENOUGH_SPACE,
    CORRUPTION,
    BLOCK_NOT_FOUNED 

}error_t;

#pragma pack(1)

typedef struct Request 
{
    Request();
    Request(uint64_t id_, unsigned char type_, uint64_t offset_, const void *block_);

    uint64_t m_id;
    unsigned char m_type; //1 == read, 2 == write
    uint64_t m_offset;
    char m_block[4096];
}Request_t;

typedef struct Reply
{
    Reply();
    Reply(uint64_t id_, unsigned char type_, error_t error_, const void *block_);

    uint64_t m_id;
    unsigned char m_type;
    error_t m_error;
    char m_block[4096];
}Reply_t;

#pragma pack()

}//ilrd


#endif // __RD94_PROTOCOL_HPP__