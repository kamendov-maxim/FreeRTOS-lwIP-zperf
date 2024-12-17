/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "net_core.h"
#include <stdio.h>

/** see header **/
#if 1
void copy_sockaddr_in6_to_sockaddr_storage(const struct sockaddr_in6 *src,
                                           struct sockaddr_storage *dest)
{
	memset(dest, 0, sizeof(struct sockaddr_storage));
    if (src->sin6_family == AF_INET6)
	{
        memcpy(dest, src, sizeof(struct sockaddr_in6));
    }
}
#endif
/** see header **/
void copy_sockaddr_in_to_sockaddr_storage(const struct sockaddr_in *src,
                                          struct sockaddr_storage *dest)
{
    memset(dest, 0, sizeof(struct sockaddr_storage));
    if (src->sin_family == AF_INET)
	{
        memcpy(dest, src, sizeof(struct sockaddr_in));
    }
}
