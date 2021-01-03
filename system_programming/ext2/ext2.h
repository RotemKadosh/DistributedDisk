#ifndef __EXT2_H__
#define __EXT2_H__

/*
DESCRIPTION:
    findes a file on the device, and prints:
        1. The superblock
        2. group descriptor
        3. File content
Param:
    dev - device name
    file path - reletive path to file 
return:
     0 - on success
     -1 -fail
*/

 int Find(const char *dev, const char *file_path);


#endif /*__EXT2_H__*/

