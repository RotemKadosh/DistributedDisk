#ifndef _UDP_H_
#define _UDP_H_

#include <netinet/in.h> /*in_addr_t*/
#define FAIL (-1)
#define SUCCESS (0)

typedef struct sockaddr_in sockaddr_t;

int CreateUdpSocket();

void InitUdpSockAddr(sockaddr_t *sockadd, in_addr_t address);


#endif /*_UDP_H_*/