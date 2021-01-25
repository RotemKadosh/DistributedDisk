#include "factory.hpp"
#include "factory_impl.hpp"
#include "utils.hpp"
#include <boost/shared_ptr.hpp> //shared_ptr
#include <string> //std::string
#include <iostream>
using namespace ilrd;
using namespace std;

class Base 
{  
public:
    explicit Base(int par_):m_a(par_){};
    int m_a;
    virtual int Get()
    {
        return m_a;
    }

};
class Derived:public Base
{  
public:
    explicit Derived(int par_):Base(par_), m_b(19){};
    int m_b;
    virtual int Get()
    {
        return m_b;
    }

};

boost::shared_ptr<Base> CreateBase(int par_)
{
    return boost::shared_ptr<Base>(new Base(par_));
}
boost::shared_ptr<Derived> CreateDerived(int par_)
{
    
    return boost::shared_ptr<Derived>(new Derived(par_));
}


test_status_t AddTest()
{
    Factory<Base,int,string> fac;
    fac.Add("Base", CreateBase);
    fac.Add("Derived", CreateDerived);
    boost::shared_ptr<Base> b = fac.Create("Base", 12);
    REQUIRE(12 == b->Get());
    boost::shared_ptr<Base> d = fac.Create("Derived", 12);
    REQUIRE(19 == d->Get());
    try
    {
        boost::shared_ptr<Base> g = fac.Create("notExist", 12);
    }
    catch(const Factory<Base,int,string>::WrongKey *e)
    {
        REQUIRE(true);
    }
    
    return PASSED;

}

int main()
{
    RUNTEST(AddTest);
    return PASSED;
}
