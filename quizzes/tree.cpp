#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <string>
#include <iostream>
#include <exception>

int tree(const char *path = ".")
{
    struct dirent **namelist;
    struct stat sb;
    int n;
    static int depth = 0;
    std::string full_path(path);
    n = scandir(path, &namelist, NULL, alphasort);
    if (n == -1) 
    {

        throw;
    }
    
    while (n--) 
    {
        int curr_depth = depth;
        std::string name(namelist[n]->d_name);
        std::cout << "|";
        while(curr_depth-- > 0)
        {
            std::cout << "-";
        }
        std::cout << name << '\n';
        if("." != name && ".." != name)
        {   
            std::string next_full_path( full_path + "/" + name);
        
            if(-1 == lstat(next_full_path.c_str(), &sb))
            {
                perror("stat:");
                std::cout << "fail\n";
                return -1;
            }
            if(S_IFDIR == (sb.st_mode & S_IFMT))
            {
                depth += 1;
                std::cout << "|";
                tree(next_full_path.c_str());
            }
            free(namelist[n]);
      
        }
    }
    free(namelist);

    return 0;
}
int main()
{
    return tree("/home/rotemkadosh27/git/projects");
}