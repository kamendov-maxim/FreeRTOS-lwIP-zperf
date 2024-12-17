/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#include "lwip/sockets.h"
#include "net/net_if.h"
#include "net/net_ip.h"

/**
 * @brief Alias for poll file descriptor
 * **/
typedef struct pollfd zsock_pollfd;

/**
 * @brief Sockey priority
 * @note This feature is not supported by lwip, so this macro has no effect
 * **/
#define SO_PRIORITY                                       (12)

/**
 * @brief Macro wrapper for MSG_DONTWAIT flag
 * **/
#define ZSOCK_MSG_DONTWAIT                                (MSG_DONTWAIT)

/**
 * @brief Macro wrapper for POLLIN state
 * **/
#define ZSOCK_POLLIN                                      (POLLIN)

/**
 * @brief Macro wrapper for POLLERR state
 * **/
#define ZSOCK_POLLERR                                     (POLLERR)

/**
 * @brief Macro wrapper for POLLNVAL state
 * **/
#define ZSOCK_POLLNVAL                                    (POLLNVAL)

/**
 * @brief Macro wrapper for lwip_socket
 * **/
#define zsock_socket(domain,type,protocol)                 lwip_socket(domain,type,protocol)

/**
 * @brief Macro wrapper for lwip_setsockopt
 * **/
#define zsock_setsockopt(s,level,optname,opval,optlen)     lwip_setsockopt(s,level,optname,opval,optlen)

/**
 * @brief Macro wrapper for lwip_connect
 * **/
#define zsock_connect(s,name,namelen)                      lwip_connect(s,name,namelen)

/**
 * @brief Macro wrapper for lwip_recvfrom
 * **/
#define zsock_recvfrom(s,mem,len,flags,from,fromlen)       lwip_recvfrom(s,mem,len,flags,from,fromlen)

/**
 * @brief Macro wrapper for lwip_recv
 * **/
#define zsock_recv(s,mem,len,flags)                        lwip_recv(s,mem,len,flags)

/**
 * @brief Macro wrapper for lwip_close
 * **/
#define zsock_close(s)                                     lwip_close(s)

/**
 * @brief Macro wrapper for lwip_bind
 * **/
#define zsock_bind(s,name,namelen)                         lwip_bind(s,name,namelen)

/**
 * @brief Macro wrapper for lwip_listen
 * **/
#define zsock_listen(s,backlog)                            lwip_listen(s,backlog)

/**
 * @brief Macro wrapper for lwip_poll
 * **/
#define zsock_poll(fds,nfds,timeout)                       lwip_poll(fds,nfds,timeout)

/**
 * @brief Macro wrapper for lwip_accept
 * **/
#define zsock_accept(s,addr,addrlen)                       lwip_accept(s,addr,addrlen)

/**
 * @brief Macro wrapper for lwip_send
 * **/
#define zsock_send(s,dataptr,size,flags)                   lwip_send(s,dataptr,size,flags)

/**
 * @brief Macro wrapper for lwip_sendto
 * **/
#define zsock_sendto(s,dataptr,size,flags,to,tolen)        lwip_sendto(s,dataptr,size,flags,to,tolen)


#endif /* __SOCKET_H */