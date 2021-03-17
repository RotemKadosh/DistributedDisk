#include "protocol_translator.hpp"


namespace ilrd
{

Request_t ProtocolTranslator::TranslateRequest(uint64_t offset_, unsigned char type_, const void *block_)
{
    static uint64_t id_conter = 0;
    return Request_t(++id_conter, type_, offset_, block_);
}

Reply_t ProtocolTranslator::TranslateReplay(uint64_t id_, unsigned char type_, const void *block_, error_t error_)
{
    return Reply_t(id_, type_,error_, block_);
}

}//ilrd

