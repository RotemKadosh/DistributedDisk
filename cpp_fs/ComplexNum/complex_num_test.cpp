#include "cmplx_num.hpp"
#include "cmplx_num_impl.hpp"
#include "../../ds/utils/test.h" // RUNTEST REQUIRE

using namespace ilrd;

test_status_t CreateTest();
test_status_t SubTest();
test_status_t CopyTest();
test_status_t MultTest();
test_status_t IOTest();
test_status_t EqualityTest();
test_status_t AdvanceTest();
test_status_t DivTest();
test_status_t AddTest();


int main()
{
    RUNTEST(CopyTest);
   /*
    RUNTEST(CreateTest);
    RUNTEST(EqualityTest);
    RUNTEST(AddTest);
    RUNTEST(SubTest);
    RUNTEST(MultTest);
    RUNTEST(DivTest);
    RUNTEST(IOTest);
    RUNTEST(AdvanceTest);
    Complex c1(1);
    Complex c2(0);
    for(int i = 0; i < 100000; i++)
    {
        c2 += c1;
    }
    */
    return PASSED;
}


test_status_t CreateTest()
{
   
    Complex c1(10, 2);
    Complex c2(0, 0);
    Complex c3(100, -172.6);
    Complex c4(-736.9, 8372.1837);
    Complex c5(7.5);

    REQUIRE(c1.GetReal() - 10 < 0.001);
    REQUIRE(c1.GetImaginary() - 2 < 0.001);
    
    REQUIRE(c2.GetReal() - 0 < 0.001);
    REQUIRE(c2.GetImaginary() - 0 < 0.001);

    REQUIRE(c3.GetReal() - 100 < 0.001);
    REQUIRE(c3.GetImaginary() - (-172.6) < 0.001);

    REQUIRE(c4.GetReal() - (-736.9) < 0.001);
    REQUIRE(c4.GetImaginary() - (8372.1837) < 0.001);
    
    REQUIRE(c5.GetReal() - 7.5 < 0.001);
    REQUIRE(c5.GetImaginary() - 0 < 0.001);
    return PASSED;
}

test_status_t EqualityTest()
{

    Complex c1(10, 2);
    Complex c11(10, 2);

    Complex c2(0, 0);
    Complex c22(0, 0);

    Complex c3(100, -172.6);
    Complex c33(100, -172.6);
    Complex c4(-736.9, 8372.1837);
    Complex c5(7.5);
    Complex c6(7.5);
    REQUIRE(c1 == c11);
    REQUIRE(c11 == c1);
    REQUIRE(c11 != c2);

     REQUIRE(c2 == c22);
    REQUIRE(c22 == c2);
    REQUIRE(c2 != c3);

     REQUIRE(c3 == c33);
    REQUIRE(c33 == c33);
    REQUIRE(c11 != c2);

     REQUIRE(c4 == c4);
    REQUIRE(c4 != c5);
    REQUIRE(c5 == c5);
    return PASSED;
}

test_status_t MultTest()
{
    Complex c1(10, 2);
    Complex c11(10, 2);
    Complex ans1(c1 * c11);
    Complex cmp1(96, 40);
    REQUIRE(ans1 == cmp1);
    Complex ans(20, 4);
    REQUIRE(ans == (2 * c1));
    Complex c2(0, 1);
    Complex c22(12, 0);
    Complex ans2(c2 * c22);
    Complex cmp2(0, 12);
    REQUIRE(ans2 == cmp2);

    Complex c3(100, -172.6);
    Complex c33(98745, -5458.6);
    Complex ans3(c3 * c33);
    Complex cmp3(8932345.64, -17589247);
    REQUIRE(ans3 == cmp3);

    Complex c4(-736.9, 8372.1837);
    Complex c44(7.5);
    Complex ans4(c4 * c44);
    Complex cmp4(-5526.75, 62791.37775);

    REQUIRE(ans4 == cmp4);
    return PASSED;

}

test_status_t DivTest()
{

    Complex cmp1(12, 5);
    Complex cmp2(1.6, 5.3);
    Complex cmp3(-2);

    Complex ans = cmp1 / cmp2;
    REQUIRE(0.00001 > ans.GetReal() - 1.4910277 && -0.00001 < ans.GetReal() - 1.4910277);
    REQUIRE(0.00001 > ans.GetImaginary() + 1.8140294 && -0.00001 < ans.GetImaginary() + 1.8140294);

    REQUIRE(-6 == (cmp1 / cmp3).GetReal());
    REQUIRE(-2.5 == (cmp1 / cmp3).GetImaginary());

    REQUIRE(-0.8 == (cmp2 / cmp3).GetReal());
    REQUIRE(-2.65 == (cmp2 / cmp3).GetImaginary());

    return PASSED;
}

test_status_t IOTest()
{
    Complex c1(9);
    Complex c2(10, -18);
    Complex c3(-9474, 892);

    std::cout << c1;
    std::cout << c2;
    std::cout << c3;
  
    std::cin >> c1;
    std::cout << c1;

    
    std::cin >> c2;

    
    std::cout << c2;

    return PASSED;
}

test_status_t AdvanceTest()
{
    Complex c1(10, 2);
    c1 *= c1;
    REQUIRE(c1 == Complex(96, 40));

    c1 *= 12;
    REQUIRE(c1 == Complex(1152,480));

    c1 += 7;
    c1 += Complex(15,-902);
    REQUIRE(c1 == Complex(1174,-422));

    c1 -= 9;
    REQUIRE(c1 == Complex(1165,-422));

    c1 -= Complex(28,89);
    REQUIRE(c1 == Complex(1137,-511));

    c1 /= 10;
    REQUIRE(c1 == Complex(113.7,-51.1));

    return PASSED;

 }

test_status_t AddTest()
{
    Complex cmp1(12, 5);
    Complex cmp2(1.6, 5.3);
    Complex cmp3(2);
    REQUIRE(13.6 == (cmp1 + cmp2).GetReal());
    REQUIRE(10.3 == (cmp1 + cmp2).GetImaginary());

    REQUIRE(14 == (cmp1 + cmp3).GetReal());
    REQUIRE(5 == (cmp1 + cmp3).GetImaginary());

    REQUIRE(3.6 == (cmp2 + cmp3).GetReal());
    REQUIRE(5.3 == (cmp2 + cmp3).GetImaginary());
    return PASSED;
}

test_status_t SubTest()
{
    Complex cmp1(12, 5);
    Complex cmp2(1.6, 5.3);
    Complex cmp3(-2);

    REQUIRE(10.4 == (cmp1 - cmp2).GetReal());
    REQUIRE(0.000001 > (cmp1 - cmp2).GetImaginary() - 0.3);

    REQUIRE(14 == (cmp1 - cmp3).GetReal());
    REQUIRE(5 == (cmp1 - cmp3).GetImaginary());

    REQUIRE(3.6 == (cmp2 - cmp3).GetReal());
    REQUIRE(5.3 == (cmp2 - cmp3).GetImaginary());

    return PASSED;
}

Complex foo(Complex c1, Complex& c2)
{
    std::cout << &c1 <<'\n';
    std::cout << c1 << '\n';
    std::cout << &c2 <<'\n';
    std::cout << c2 << '\n';

    return c1;
}


test_status_t CopyTest()
{
    Complex c1 = Complex(12,12);
    Complex c2 = foo(c1,c1);
    return PASSED;
}