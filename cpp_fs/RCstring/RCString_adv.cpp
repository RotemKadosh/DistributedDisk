
#include <cstring>//strlen, memcpy, strcpy,strcat
#include <cassert>//static assert
#include <iostream> // std 
#include <iomanip> //setw

#include "RCString_adv.hpp"

namespace ilrd
{

RCString::RCString(const char *str_) throw(std::bad_alloc): 
m_data(StringRef::CreateStringRef(str_))
{}

RCString::RCString(const RCString& other_) throw(std::bad_alloc):
m_data(other_.m_data)
{
    ++m_data->m_ref_count;
}

RCString& RCString::operator=(const RCString& other_)
{
    StringRef *old_string_ref = m_data;
    m_data = other_.m_data;
    ++(m_data->m_ref_count);

    --(old_string_ref->m_ref_count);
    if (0 == old_string_ref->m_ref_count)
    {
        delete []old_string_ref;
    }

    return *this;
}

RCString::~RCString()
{
    --m_data->m_ref_count;
    if(0 == m_data->m_ref_count)
    {
        delete []m_data;
    }   
}

bool RCString::IsShared() const
{
    return (1 < m_data->m_ref_count);
}

const char* RCString::ToCStr() const
{
    return const_cast<const char *>(m_data->m_ref_str);

} 

size_t RCString::Length() const
{
    return strlen(ToCStr());
}

RCString& RCString::Concat(const RCString& other_) throw(std::bad_alloc)// throws bad_alloc
{
    size_t new_length = Length() + other_.Length() + NULL_BYTE;
    char *new_str = new char[new_length];

    strcpy(new_str, m_data->m_ref_str);
    strcat(new_str, other_.m_data->m_ref_str);

    if(IsShared())
    {
        --(m_data->m_ref_count);    
    }
    else
    {
        delete []m_data;
    }

    m_data = StringRef::CreateStringRef(new_str);
    delete []new_str;
    return *this;
} 

char RCString::operator[](size_t pos_) const
{
    return m_data->m_ref_str[pos_];
}

RCString::CharProxy RCString::operator[](size_t pos_)
{
    return CharProxy(*this, pos_);
}

std::ostream& operator<<(std::ostream& os_, const RCString& str_)
{
   return os_<<str_.ToCStr();
}

std::istream& operator>>(std::istream& is_, RCString& str_)
{
    const size_t max_line_size = 1024;
    char buffer[max_line_size] = {0};
    is_ >> std::setw(max_line_size) >> buffer;
    str_ = RCString(buffer);
    return is_; 
}

bool operator==(const RCString& str_, const RCString& str_to_compare_)
{
    return (0 == strcmp(str_.ToCStr(), str_to_compare_.ToCStr()));
}

bool operator>(const RCString& str_, const RCString& str_to_compare_)
{
    return (0 < strcmp(str_.ToCStr(), str_to_compare_.ToCStr()));
}

bool operator<(const RCString& str_, const RCString& str_to_compare_)
{
    return (0 > strcmp(str_.ToCStr(), str_to_compare_.ToCStr()));
}

RCString operator+(const RCString& str_, const RCString& other_) throw(std::bad_alloc)
{
    RCString result = RCString(str_);
    result.Concat(other_);
    return result;
}

//**************StringRef***********
RCString::StringRef::StringRef(const char *other_, size_t length_) throw(std::bad_alloc):
m_ref_count(1),
m_ref_str(reinterpret_cast<char *>(reinterpret_cast<char *>(&m_ref_str) + sizeof(char * )))
{
    memcpy(m_ref_str, other_, length_);
}


RCString::StringRef *RCString::StringRef::CreateStringRef(const char *str_)
{
    size_t length = strlen(str_) + NULL_BYTE;
    char *placement_new = new char[sizeof(StringRef) + length];
   
    StringRef* string_ref = new (placement_new) StringRef(str_, length);
    return (string_ref);  
}

//**************proxy***********

RCString::CharProxy::CharProxy(RCString& str_, size_t index_):
m_string(str_),
m_index(index_)
{}

RCString::CharProxy::CharProxy(const CharProxy& other_):
m_string(other_.m_string),
m_index(other_.m_index)
{}


RCString::CharProxy& RCString::CharProxy::operator=(const CharProxy& other_) throw(std::bad_alloc)
{
    if(m_string.IsShared())
    {
        --m_string.m_data->m_ref_count;
        m_string.m_data = RCString::StringRef::CreateStringRef(m_string.ToCStr());
    }
    m_string.m_data->m_ref_str[m_index] = other_.m_string.m_data->m_ref_str[other_.m_index];
    return *this;
}

RCString::CharProxy& RCString::CharProxy::operator=(char c_) throw(std::bad_alloc)
{
    if(m_string.IsShared())
    {
        --m_string.m_data->m_ref_count;
        m_string.m_data = RCString::StringRef::CreateStringRef(m_string.ToCStr());
    }
    *(m_string.m_data->m_ref_str + m_index) = c_;
    return *this;
}

RCString::CharProxy::operator char()
{
    return *(m_string.m_data->m_ref_str + m_index);
}

RCString::CharProxy::~CharProxy()
{}

}



