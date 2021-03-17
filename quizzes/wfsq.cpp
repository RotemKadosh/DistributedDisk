#include "WFSQ.hpp"
#include "../projects/fs_project/utils/utils.hpp"
#include <iostream>

int main()
{
    int i = 0;
    Wfsq<int> q(1);
    q.Push(i);
    REQUIRE(false == q.Push(i, boost::chrono::seconds(2)));
    std::cout << "ewajhbfaw\n";
    
    
    return PASSED;

}