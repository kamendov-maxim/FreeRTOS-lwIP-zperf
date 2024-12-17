/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "net_private.h"
#include <assert.h>
#include <stdio.h>

/** see header **/
char * net_sprint_ipv4_addr(const struct in_addr * addr)
{
    assert(addr != NULL);

    ip4_addr_t local_addr;
    IN4_ADDR_TO_IP4_ADDR(&local_addr, addr);

    return ip4addr_ntoa(&local_addr);
}

#if 1
/** see header **/
char * net_sprint_ipv6_addr(const struct in6_addr * addr)
{
    assert(addr != NULL);

    ip6_addr_t local_addr;
    IN6_ADDR_TO_IP6_ADDR(&local_addr, addr);

    return ip6addr_ntoa(&local_addr);
}
#endif
