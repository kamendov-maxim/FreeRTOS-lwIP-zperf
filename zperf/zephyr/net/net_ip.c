/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <assert.h>

#include "util.h"
#include "net_ip.h"
#include "config.h"

/**
 * @brief Convert port passed as string literal to number
 * @param buf port as string literal
 * @param port number where converted port will be stored
 * @return true - convert has been succefull
 *         false - convert has not been succefull
 * **/
static bool convert_port(const char *buf,
                         uint16_t *port)
{
	unsigned long tmp = 0;
	char *endptr = NULL;

	tmp = strtoul(buf, &endptr, 10);
	if ((endptr == buf && tmp == 0) ||
	    !(*buf != '\0' && *endptr == '\0') ||
	    ((unsigned long)(unsigned short)tmp != tmp))
    {
		return false;
	}

	*port = tmp;

	return true;
}

/**
 * @brief Convert port passed as string literal to number
 * @param buf port as string literal
 * @param port number where converted port will be stored
 * @return true - convert has been succefull
 *         false - convert has not been succefull
 * **/
//#if defined(CONFIG_NET_IPV6)
#if 0
static bool parse_ipv6(const char *str,
                       size_t str_len,
		               struct sockaddr *addr,
                       bool has_port)
{
	char *ptr = NULL;
	struct in6_addr *addr6;
	char ipaddr[INET6_ADDRSTRLEN + 1];
	int end = 0, len = 0, ret = 0, i = 0;
	uint16_t port = 0;

	len = MIN(INET6_ADDRSTRLEN, str_len);

	for (i = 0; i < len; i++)
    {
		if (!str[i])
        {
			len = i;
			break;
		}
	}

	if (has_port)
    {
		/* IPv6 address with port number */
		ptr = memchr(str, ']', len);
		if (!ptr)
        {
			return false;
		}

		end = MIN(len, ptr - (str + 1));
		memcpy(ipaddr, str + 1, end);
	}
    else
    {
		end = len;
		memcpy(ipaddr, str, end);
	}

	ipaddr[end] = '\0';

	addr6 = &net_sin6(addr)->sin6_addr;

	ret = net_addr_pton(AF_INET6, ipaddr, addr6);
	if (ret < 0)
    {
		return false;
	}

	net_sin6(addr)->sin6_family = AF_INET6;

	if (!has_port)
    {
		return true;
	}

	if ((ptr + 1) < (str + str_len) && *(ptr + 1) == ':')
    {
		/* -1 as end does not contain first [
		 * -2 as pointer is advanced by 2, skipping ]:
		 */
		len = str_len - end - 1 - 2;

		ptr += 2;

		for (i = 0; i < len; i++)
        {
			if (!ptr[i])
            {
				len = i;
				break;
			}
		}

		/* Re-use the ipaddr buf for port conversion */
		memcpy(ipaddr, ptr, len);
		ipaddr[len] = '\0';

		ret = convert_port(ipaddr, &port);
		if (!ret)
        {
			return false;
		}

		net_sin6(addr)->sin6_port = htons(port);
	}
	return true;
}
#else
static inline bool parse_ipv6(const char *str,
                              size_t str_len,
			                  struct sockaddr *addr,
                              bool has_port)
{
	return false;
}
#endif /* CONFIG_NET_IPV6 */

/**
 * @brief Convert port passed as string literal to number
 * @param buf port as string literal
 * @param port number where converted port will be stored
 * @return true - convert has been succefull
 *         false - convert has not been succefull
 * **/
#if defined(CONFIG_NET_IPV4)
static bool parse_ipv4(const char *str,
                       size_t str_len,
	      	           struct sockaddr *addr,
                       bool has_port)
{
	char *ptr = NULL;
	char ipaddr[NET_IPV4_ADDR_LEN + 1];
	struct in_addr *addr4;
	int end = 0, len = 0, ret = 0, i = 0;
	uint16_t port = 0;

	len = MIN(NET_IPV4_ADDR_LEN, str_len);

	for (i = 0; i < len; i++)
    {
		if (!str[i])
        {
			len = i;
			break;
		}
	}

	if (has_port)
    {
		/* IPv4 address with port number */
		ptr = memchr(str, ':', len);
		if (!ptr)
        {
			return false;
		}

		end = MIN(len, ptr - str);
	} else
    {
		end = len;
	}

	memcpy(ipaddr, str, end);
	ipaddr[end] = '\0';

	addr4 = &net_sin(addr)->sin_addr;

	ret = net_addr_pton(AF_INET, ipaddr, addr4);
	if (ret < 0)
    {
		return false;
	}

	net_sin(addr)->sin_family = AF_INET;

	if (!has_port)
    {
		return true;
	}

	memcpy(ipaddr, ptr + 1, str_len - end);
	ipaddr[str_len - end] = '\0';

	ret = convert_port(ipaddr, &port);
	if (!ret)
    {
		return false;
	}

	net_sin(addr)->sin_port = htons(port);

	return true;
}
#else
static inline bool parse_ipv4(const char *str,
                              size_t str_len,
			                  struct sockaddr *addr,
                              bool has_port)
{
	return false;
}
#endif /* CONFIG_NET_IPV4 */

/** see header **/
bool net_ipv4_addr_cmp(const struct in_addr * first_address,
					   const struct in_addr * second_address)
{
    assert(first_address != NULL);
    assert(second_address != NULL);

    return (first_address->s_addr == second_address->s_addr);
}

/** see header **/
bool net_ipv6_addr_cmp(const struct in6_addr * first_address,
					   const struct in6_addr * second_address)
{
    assert(first_address != NULL);
    assert(second_address != NULL);

    return (memcmp(first_address->s6_addr,
                   second_address->s6_addr,
                   sizeof(first_address->s6_addr)) == 0);
}

/** see header **/
void net_ipaddr_copy(struct in_addr * dest,
				     const struct in_addr * src)

{
    assert(dest != NULL);
    assert(src != NULL);
    memcpy(&dest->s_addr, &src->s_addr, sizeof(dest->s_addr));
}

/** see header **/
void net_ipaddr6_copy(struct in6_addr * dest,
				      const struct in6_addr * src)

{
    assert(dest != NULL);
    assert(src != NULL);
    memcpy(dest->s6_addr, src->s6_addr, sizeof(dest->s6_addr));
}

/** see header **/
bool net_ipaddr_parse(const char *str,
					  size_t str_len,
			      	  struct sockaddr *addr)
{
    int i = 0, count = 0;

	if (!str || str_len == 0)
    {
		return false;
	}

	/* We cannot accept empty string here */
	if (*str == '\0')
    {
		return false;
	}

	if (*str == '[')
    {
		return parse_ipv6(str, str_len, addr, true);
	}

	for (count = i = 0; i < str_len && str[i]; i++)
    {
		if (str[i] == ':')
        {
			count++;
		}
	}

	if (count == 1)
    {
		return parse_ipv4(str, str_len, addr, true);
	}

#if defined(CONFIG_NET_IPV4) && defined(CONFIG_NET_IPV6)
	if (!parse_ipv4(str, str_len, addr, false))
    {
		return parse_ipv6(str, str_len, addr, false);
	}

	return true;
#endif

#if defined(CONFIG_NET_IPV4) && !defined(CONFIG_NET_IPV6)
	return parse_ipv4(str, str_len, addr, false);
#endif

#if defined(CONFIG_NET_IPV6) && !defined(CONFIG_NET_IPV4)
	return parse_ipv6(str, str_len, addr, false);
#endif
	return false;
}
