/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifndef __NET_IF_H
#define __NET_IF_H

#include "lwip/netif.h"
#include "lwip/inet.h"


/**
 * @brief Infinite lifetime indicate that an IPv6 Neighbor Discovery (ND)
 *        cache entry or an IPv6 prefix has an infinite lifetime
 * **/
#define NET_IPV6_ND_INFINITE_LIFETIME (0xFFFFFFFF)

/**
 * @brief Alias for IP address of network interface
 * **/
typedef ip_addr_t net_if_addr;

/**
 * @brief Struct holding IPv6 prefix
 * @note In lwip ipv6 prefix is not needed.
 * **/
struct net_if_ipv6_prefix
{

};

/**
 * @brief Return pointer to zperf network interface
 * @return pointer to zperf netif
 * **/
struct netif * net_if_get_default();

/**
 * @brief Add IPv6 address to the zperf network interface 
 * @param iface pointer to zperf network interface
 * @param addr pointer to the IPv6 address
 * @param addr_type address type (not used)
 * @param life_time validity time for the address (not used)
 * @return pointer to the network interface address
 * 
 * @note Arguments which are not used have no usefull usage in NXP lwip
 * **/
net_if_addr * net_if_ipv6_addr_add(struct netif *iface,
                                   const struct in6_addr *addr,
                                   int addr_type,
                                   uint32_t life_time);

/**
 * @brief Add IPv4 address to the zperf network interface 
 * @param iface pointer to zperf network interface
 * @param addr pointer to the IPv4 address
 * @param addr_type address type (not used)
 * @param life_time validity time for the address (not used)
 * @return pointer to the network interface address
 * 
 * @note Arguments which are not used have no usefull usage in NXP lwip
 * **/
net_if_addr * net_if_ipv4_addr_add(struct netif *iface,
                                   struct in_addr *addr,
                                   int addr_type,
                                   uint32_t life_time);


/**
 * @brief Add prefix for IPv6 address
 * @param iface pointer to zperf network interface (not used)
 * @param addr pointer to the IPv6 address (not used)
 * @param prefix_len prefix lenthg (not used)
 * @param life_time validity time for the address (not used)
 * @return pointer to the network interface address
 * 
 * @warning In lwip there is no need to set prefix for ipv6 address
 * **/
struct net_if_ipv6_prefix * net_if_ipv6_prefix_add(struct netif *iface,
                                            struct in6_addr *addr,
                                            uint8_t prefix_len,
                                            uint32_t life_time);

#endif /* __NET_IF_H */
