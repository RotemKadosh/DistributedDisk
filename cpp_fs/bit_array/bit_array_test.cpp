#include "bit_array.hpp"
#include "bit_array_impl.hpp"
#include "../utils/utils.hpp"
#include <exception>
#include <iostream>

test_status_t TestProxy();
test_status_t TestSetBinOps();
test_status_t TestEqOps();
test_status_t TestCount();
test_status_t TestFlip();
test_status_t TestToString();

using namespace ilrd;
using namespace std;

int main()
{
    RUNTEST(TestSetBinOps);
    RUNTEST(TestEqOps);
    RUNTEST(TestCount);
    RUNTEST(TestFlip);
    RUNTEST(TestToString);
    RUNTEST(TestProxy);

    return 0;
}

test_status_t TestProxy()
{
    BitArray<6344> a;
    a.Set(643, true);
    if (a[643])
    {
        a[643] = a[642] = a[641];
    }

    if(!a[643])
    {
        a[643] = true;
    }

    if (a[643])
    {
        cout << "arr[] == true worked" << endl;
    }

    a = a;

    if (a == a)
    {
        cout << "== works" << endl;
    }

    if (a != a)
    {
        cout << "!= not working" << endl;
    }

    return PASSED;
}

test_status_t TestToString()
{
    BitArray<45> a;
    cout << a.ToString() << endl;

    a.Set();
    cout << a.ToString() << endl;

    a.Set(34, 0);
    a.Set(42, 0);
    cout << a.ToString() << endl;

    BitArray<> b;
    for (int i = 0; i < 64; i += 2)
    {
        b.Set(i, true);
    }

    cout << b.ToString() << endl;

    return PASSED;
}

test_status_t TestFlip()
{
    BitArray<65> a;
    a.Set(3, true);
    a.Set(5, true);
    REQUIRE(2 == a.Count());
    a.Flip();
    cout << a.ToString() << endl;
    REQUIRE(63 == a.Count());
    REQUIRE(false == a[3]);
    REQUIRE(false == a[5]);

    BitArray<43> b;
    for (int i = 0; i < 43; i += 2)
    {
        b.Set(i, true);
    }
    cout << "printing before flip:" << endl << b.ToString() << endl;
    b.Flip();
    cout << "after flip: " << endl << b.ToString() << endl;

    return PASSED;
}
test_status_t TestCount()
{
    BitArray<45> a;
    REQUIRE(0 == a.Count());
    a.Set();
    REQUIRE(45 == a.Count());
    a.Set(34, 0);
    a.Set(42, 0);
    REQUIRE(43 == a.Count());
    a.Set(42, 1);
    REQUIRE(44 == a.Count());
    
    BitArray<65> b;
    REQUIRE(0 == b.Count());
    b.Set();
    REQUIRE(65 == b.Count());
    b.Set(34, 0);
    b.Set(42, 0);
    REQUIRE(63 == b.Count());
    b.Set(42, 1);
    REQUIRE(64 == b.Count());
    

    BitArray<128> c;
    c.Set();
    REQUIRE(128 == c.Count());

    //BitArray<43456> d;
    //d.Set();
   // REQUIRE(43456 == d.Count());

    return PASSED;

}
test_status_t TestEqOps()
{
    BitArray<> a;
    BitArray<> b;
    a.Set();
    REQUIRE(a != b);
    b.Set();
    REQUIRE(a == b);
    

    return PASSED;
}
test_status_t TestSetBinOps()
{
    BitArray<> a;
    BitArray<> b;
    a.Set();
    REQUIRE(1 == a[0]);
    REQUIRE(1 == a[63]);
    a &= b;
    REQUIRE(0 == a[0]);
    REQUIRE(0 == a[63]);

    b.Set();
    a |= b;
    REQUIRE(1 == a[0]);
    REQUIRE(1 == a[63]);


    return PASSED;
}
test_status_t Phase1()
{
    BitArray<20LU> b1;
    BitArray<640LU> b2;


    b1[1] = true;
    REQUIRE(true == b1[1]);
    b2[65] = true;
    REQUIRE(true == b2[65]);

    b2[634] = true;
    REQUIRE(true == b2[634]);
    b1[1] = false;
    b2[634] = b1[1];
    REQUIRE(false == b2[634]);
    b2[634] = b2[634]; 
    REQUIRE(false == b2[634]);
    b1[1] = true;
    b1[1] = b1[1];
    REQUIRE(true ==  b1[1]);

    try
    {
        b1[21] = true;
    }
    catch(const std::out_of_range& e)
    {
        REQUIRE(true);
    }
    

    return PASSED;
}



