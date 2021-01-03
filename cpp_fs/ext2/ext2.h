#ifndef __Dhcp_H__
#define __Dhcp_H__

#include <stddef.h> /* size_t */


/*DESCRIPTION: 
* find a file in the device.
* prints metadata about it.
*   @param
*       device_name
*       file_name
*       
*   @return
* 
*/
void FindFile(const char *device_name, const char *file_name);



void PrintSuperBlock(const char *dev);
void PrintGroupDescriptor(const char *dev, const char *file_name);
const char *GetFileContent(const char *dev, const char *file_name);



#endif /*__Dhcp_H__*/