/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __NET_IP_H
#define __NET_IP_H

#include <stdbool.h>

#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "socket.h"

/** 
 * @brief Unspecified ip6v address 
 * **/
static const struct in6_addr IPV6_ANY_ADDR = IN6ADDR_ANY_INIT;

/** 
 * @brief Lengh of ipv4 address 
 * **/
#define NET_IPV4_ADDR_LEN sizeof("xxx.xxx.xxx.xxx")

/** 
 * @brief Lengh of ipv6 address 
 * **/
#define NET_IPV6_ADDR_LEN sizeof("xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx")

/**
 * @brief Represents address type
 * @note This is not neede in lwip enviroment, only defined to make zperf compatible
 * **/
#define NET_ADDR_MANUAL                             (0)

/**
 * @brief Defines how many priority values there are
 * @note In lwip is no possibilty to prioritize net flow, so this macro has no usage
 * **/
#define NET_MAX_PRIORITIES                          (8)

/**
 * @brief Safe cast in6_addr to ip6_addr_t
 * **/
#define IN6_ADDR_TO_IP6_ADDR(ip6addr, in6addr)  	IP6_ADDR(ip6addr,                   \
                                                             (in6addr)->un.u32_addr[0], \
															 (in6addr)->un.u32_addr[1], \
															 (in6addr)->un.u32_addr[2], \
															 (in6addr)->un.u32_addr[3])

/**
 * @brief Safe cast in_addr to ip4_addr_t
 * **/
#define IN4_ADDR_TO_IP4_ADDR(ip4addr, in4addr)      ((ip4addr)->addr = (in4addr)->s_addr)

/** @brief Protocol numbers from IANA/BSD */
enum net_ip_protocol
{
	nip_IPPROTO_IP = IPPROTO_IP,            /** @brief IP protocol (pseudo-val for setsockopt() */
	nip_IPPROTO_ICMP = IPPROTO_ICMP,        /** @brief ICMP protocol   */
	nip_IPPROTO_TCP = IPPROTO_TCP,          /** @brief TCP protocol    */
	nip_IPPROTO_UDP = IPPROTO_UDP,          /** @brief UDP protocol    */
    nip_IPPROTO_IPV6 = LWIP_IPV6,           /** @brief IPv6 protocol   */
    nip_IPPROTO_ICMPV6 = IPPROTO_ICMPV6,    /** @brief ICMPv6 protocol */
	nip_IPPROTO_RAW = IPPROTO_RAW,          /** @brief RAW IP packets  */
};

/**
 * @brief Casts socket address to ipv6 version
 * @param addr pointer to socket address
 * @return casted address
 * **/
static inline struct sockaddr_in6 *net_sin6(const struct sockaddr *addr)
{
	return (struct sockaddr_in6 *)addr;
}

/**
 * @brief Casts socket address to ipv4 version
 * @param addr pointer to socket address
 * @return casted address
 * **/
static inline struct sockaddr_in *net_sin(const struct sockaddr *addr)
{
	return (struct sockaddr_in *)addr;
}

/**
 * @brief Returns pointer to unspecified IPv6 address
 * @return pointer to unspecified IPv6 address
 * **/
static inline const struct in6_addr * net_ipv6_unspecified_address()
{
   return &IPV6_ANY_ADDR;
}

/**
 * @brief Converts string represenation of IP address to binary represantation based on family
 * @param family socket adress family type
 * @param src string representation of IP
 * @param dst pointer to destination where ip adress should be converted
 * @return 0 - successed
 *         1 - failed
 * **/
static inline int net_addr_pton(sa_family_t family,
								const char * src,
				  				void * dst)
{
    return (inet_pton(family, src, dst) == 0) ? -1 : 0;
}

/**
 * @brief Compare two IPv4 addresses if they are same
 * @param first_address first adress to compare
 * @param second_address second adress to compare
 * @return true - ip addresses are same
 *         false - ip addresses are not same
 * **/
bool net_ipv4_addr_cmp(const struct in_addr * first_address,
					   const struct in_addr * second_address);


/**
 * @brief Compare two IPv6 addresses if they are same
 * @param first_address first adress to compare
 * @param second_address second adress to compare
 * @return true - ip addresses are same
 *         false - ip addresses are not same
 * **/
bool net_ipv6_addr_cmp(const struct in6_addr * first_address,
					   const struct in6_addr * second_address);

/**
 * @brief Copy one address to another address (IPv4)
 * @param dest detination address
 * @param src source address
 * @return non
 * **/
void net_ipaddr_copy(struct in_addr * dest,
				     const struct in_addr * src);

/**
 * @brief Copy one address to another address (IPv6)
 * @param dest detination address
 * @param src source address
 * @return non
 * **/
void net_ipaddr6_copy(struct in6_addr * dest,
					  const struct in6_addr * src);

/**
 * @brief Parse string to the IP address (IPv4/6)
 * @param str source address
 * @param str_len lengh of source address
 * @param addr pointer to the binary reprasantion of address
 * @return true - parse was succesfull
 * 		   false - parse was not succesfull
 * **/
bool net_ipaddr_parse(const char *str,
					  size_t str_len,
			      	  struct sockaddr *addr);

/**
 * @brief Check if IPv4 address in unspecified (0.0.0.0)
 * @param addr address to check
 * @return true - address is unspecified
 *         false - address is not unspecified
 * **/
static inline bool net_ipv4_is_addr_unspecified(const struct in_addr * addr)
{
	 return (addr == NULL || addr->s_addr == IPADDR_ANY);
}

/**
 * @brief Check if IPv6 address in unspecified (::)
 * @param addr address to check
 * @return true - address is unspecified
 *         false - address is not unspecified
 * **/
static inline bool net_ipv6_is_addr_unspecified(const struct in6_addr * addr)
{
	return (net_ipv6_addr_cmp(addr, net_ipv6_unspecified_address()));
}


#endif /** __NET_IP_H **/
