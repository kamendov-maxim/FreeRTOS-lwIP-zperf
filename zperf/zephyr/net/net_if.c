/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include "lwipopts.h"
#include "net_if.h"

#include "net_ip.h"

void sys_lock_tcpip_core(void);
#define LOCK_TCPIP_CORE()          sys_lock_tcpip_core()
void sys_unlock_tcpip_core(void);
#define UNLOCK_TCPIP_CORE()        sys_unlock_tcpip_core()


/**
 * @brief Default network interface
 * @note  This should be used in application as main and only network interface for zperf
 * **/
static struct netif default_net_intrface;

/**
 * @brief Instance of ipv6 deafult prefix
 * **/
static struct net_if_ipv6_prefix default_net_prefix;

/** see header **/
struct netif * net_if_get_default()
{
    return &default_net_intrface;
}

#if 1
/** see header **/
net_if_addr * net_if_ipv6_addr_add(struct netif *iface,
                                   const struct in6_addr *addr,
                                   int addr_type,
                                   uint32_t life_time)
{
    (void)addr_type;
    (void)life_time;

    assert(iface != NULL);
    assert(addr != NULL);

    net_if_addr * res = NULL;    

    ip6_addr_t local_addr;
    IN6_ADDR_TO_IP6_ADDR(&local_addr, addr);
    
    LOCK_TCPIP_CORE();
    if (netif_add_ip6_address(iface,
	                          &local_addr,
							  NULL) == ERR_OK)
    {
        res = (iface->ip6_addr);
    }
    UNLOCK_TCPIP_CORE();

    return res;
}
#endif
/** see header **/
net_if_addr * net_if_ipv4_addr_add(struct netif *iface,
                                   struct in_addr *addr,
                                   int addr_type,
                                   uint32_t life_time)
{
    (void)addr_type;
    (void)life_time;

    assert(iface != NULL);
    assert(addr != NULL);

    ip4_addr_t local_addr;
    IN4_ADDR_TO_IP4_ADDR(&local_addr, addr);
   
#if 0
    LOCK_TCPIP_CORE();
#endif
    netif_set_addr(iface, &local_addr, NULL, NULL);
#if 0
    UNLOCK_TCPIP_CORE();
#endif
   
    return (&iface->ip_addr);
}

/** see header **/
struct net_if_ipv6_prefix * net_if_ipv6_prefix_add(struct netif *iface,
                                     struct in6_addr *addr,
                                     uint8_t prefix_len,
                                     uint32_t life_time)
{
    (void)iface;
    (void)addr;
    (void)prefix_len;
    (void)life_time;

    return (&default_net_prefix);
}
