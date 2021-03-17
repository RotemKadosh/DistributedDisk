#ifndef __RD94_PROTOCOL_TRANSLATOR_HPP__
#define __RD94_PROTOCOL_TRANSLATOR_HPP__


#include <stdint.h>//uint64_t
#include "app_protocol.hpp"

namespace ilrd
{

class ProtocolTranslator // why not just a function
{
public:
    static Request_t TranslateRequest(uint64_t offset_, unsigned char type_, const void *block_);
    static Reply_t TranslateReplay(uint64_t id_, unsigned char type_, const void *block_, error_t error_);
private:
    
};


}//ilrd

#endif//  __RD94_PROTOCOL_TRANSLATOR_HPP__
