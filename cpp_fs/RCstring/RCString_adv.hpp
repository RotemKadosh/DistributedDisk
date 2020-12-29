#ifndef ILRD_94_RCSTRING_HPP
#define ILRD_94_RCSTRING_HPP

#include <iosfwd> // size_t
#include <new> //bas_alloc
#if cplusplus >= 201103L 
static_assert(1 == sizeof(char) , "Only works if 1==sizeof(char)");
#else
namespace { typedef int Stupid[ 1 / (1==sizeof(char)) ]; }// compilation error if  1!=sizeof(char), otherwise does nothing
#define noexcept throw()
#endif // #if cplusplus >= 201103L 

#if __cplusplus<201104
#define noexcept throw()
#endif //#if __cplusplus<201104

namespace ilrd
{

class RCString
{
public:
    RCString(const char *str_ = "") throw(std::bad_alloc);  // non explicit on purpose, throws bad_alloc
    RCString(const RCString& other_) throw(std::bad_alloc); // throws bad_alloc
    RCString& operator=(const RCString& other_);
    ~RCString();          
       
    RCString& Concat(const RCString& other_) throw(std::bad_alloc);   // throws bad_alloc
    const char* ToCStr() const;  
    bool IsShared() const;
    size_t Length() const; 

    char operator[](size_t pos_) const; 
    

private:

    static const size_t NULL_BYTE = 1;

    class CharProxy 
    {
    public:
        CharProxy(RCString& str_, size_t index_);
        CharProxy(const CharProxy& other_);
        CharProxy& operator=(const CharProxy& other_) throw(std::bad_alloc);
        CharProxy& operator=(char c_) throw(std::bad_alloc); // change single char.
        operator char();
        ~CharProxy();
        
    private:
        
        RCString& m_string;
        const size_t m_index;
    };

    struct StringRef
    {
    public:
        StringRef(const char *str_ , size_t length_) throw(std::bad_alloc); // throws bad_alloc
        static StringRef *CreateStringRef(const char *str_);
        void UseRef();
        void UnUseRef();
        size_t m_ref_count; 
        char *m_ref_str; 
    
    private:
        StringRef& operator=(StringRef &other_);
        StringRef(const StringRef &other_);   
    }; 
    StringRef *m_data;

public:

CharProxy operator[](size_t pos_);
    
};

std::ostream& operator<<(std::ostream& os_, const RCString& str_);
std::istream& operator>>(std::istream& is, RCString& str_);
bool operator==(const RCString& str_, const RCString& str_to_compare_);
bool operator>(const RCString& str_, const RCString& str_to_compare_);
bool operator<(const RCString& str_, const RCString& str_to_compare_);
RCString operator+(const RCString& str_, const RCString& other_) throw(std::bad_alloc); 

} //namespace ilrd

#endif  // ILRD_94_RCSTRING_HPP


