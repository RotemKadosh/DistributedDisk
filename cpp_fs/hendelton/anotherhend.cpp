#include <iostream>
#include <dlfcn.h>
#include "/home/rotemkadosh27/git/projects/fs_project/singlton/singleton_impl.hpp"
#include "/home/rotemkadosh27/git/projects/fs_project/logger/logger.hpp"
using namespace ilrd;

int main() 
{
    using std::cout;
    using std::cerr;
    using std::endl;

    cout << "address of logger in main : " << Singleton<Logger>::GetInstance() << endl;// call singleton

    // open the library
    void* handle = dlopen("./libother.so", RTLD_LAZY);

    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // load the symbol
    typedef void (*hello_t)();

    // reset errors
    dlerror();
    hello_t hello = (hello_t) dlsym(handle, "print");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'hello': " << dlerror() << '\n';
        dlclose(handle);
        return 1;
    }

    hello(); // call plugin function hello

    cout << "address of logger in main : " << Singleton<Logger>::GetInstance() << endl;// call singleton
    dlclose(handle);
}