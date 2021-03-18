#include "config_manager.hpp"
#include "singleton.hpp"
#include <iostream>

int main()
{
    using namespace ilrd;
    
    std::string path("this_is_my_path");
    if(GET_CONFIG("PATH") == path)
    {
        std::cout<<"passed path";
    } 
     std::string val("this is values");
     if(GET_CONFIG("VAL@") == val)
    {
        std::cout<<"passed val";
    } 
    return 0;
}