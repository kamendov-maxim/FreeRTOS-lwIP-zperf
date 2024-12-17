/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */


/*

    TOTO UPRAVIT V CMAKE TAK ABY SME NEMUSELI VOLAT CEZ CELU CESTU

*/
#ifndef __NET_PRIVATE_H
#define __NET_PRIVATE_H

#include "lwip/netif.h"
#include "lwip/inet.h"

#include "net_ip.h"

/**
 * @brief Wrapper for ip4addr_ntoa
 * @param addr pointer to address which should be converted
 * @return pointer to ip address literal
 * **/
char * net_sprint_ipv4_addr(const struct in_addr * addr);

/**
 * @brief Wrapper for ip6addr_ntoa
 * @param addr pointer to address which should be converted
 * @return pointer to ip address literal
 * **/
char * net_sprint_ipv6_addr(const struct in6_addr * addr);


#endif /* __NET_PRIVATE_H */
