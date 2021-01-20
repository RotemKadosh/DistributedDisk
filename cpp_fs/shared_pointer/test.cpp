
#include "shared_ptr_impl.hpp"




using namespace ilrd;

class A
{
public:
    static const int a = 10;
};

class B: public A
{
private:
    static const int b = 15;
};
class c
{
private:
    int c;

};

test_status_t t1()
{

    SharedPtr<int> p1(new int(5));
    SharedPtr<int> p3(p1);
    SharedPtr<int> p4(new int(6));

    REQUIRE((5 == *p1));
    REQUIRE((5 == *p3));
    REQUIRE((*p3 == *p1));
    p3 = p4;
    REQUIRE((6 == *p3));
    p1 = p3;
    REQUIRE((6 == *p1));


    SharedPtr<char> p2(new char('a'));
    
    REQUIRE(('a' == *p2));
 
    SharedPtr<A> p5(new A());
    REQUIRE((p5->a) == 10);
    SharedPtr<B> p6(new B());
    SharedPtr<A> p7(p6);

    REQUIRE(p6->a == p7->a);
    return PASSED;
}
int main()
{
    RUNTEST(t1);
    return PASSED;
}