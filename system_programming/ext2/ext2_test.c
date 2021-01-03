#include "ext2.h"




int main()
{   
    printf("-------test 1--root dir-----------\n ");
    Find("/dev/ram0", "/new.txt");
 
    printf("-------test 2--nested dir-----------\n ");
    Find("/dev/ram0", "/new_dir/new_dir.txt");
    
    printf("-------test 3--not exist dir-----------\n ");
    Find("/dev/ram0", "/tmp/ramdisk/new_dir/not_exist_dir/new");
    
    return 0;
}