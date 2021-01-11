#ifndef _TCP_H_
#define _TCP_H_

#include <netinet/in.h> /*in_addr_t*/

#define FAIL (-1)
#define SUCCESS (0)

#define TRUE (1)
#define FALSE (0)

typedef struct sockaddr_in sockaddr_t;

int CreateTcpSocket();

void InitTcpSockAddr(sockaddr_t *sockadd, in_addr_t address, int port);


#endif /*_TCP_H_*/