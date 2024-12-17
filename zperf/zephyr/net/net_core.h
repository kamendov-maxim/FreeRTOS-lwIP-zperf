/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __NET_CORE_H
#define __NET_CORE_H

#include "lwip/sockets.h"

/**
 * @brief Safe cast sockaddr_in6 to sockaddr_storage
 * @param src pointer to source address
 * @param dest pointer to desination address
 * @return non
 * 
 * @note This function was created due to platform issue. Instead of using sockaddr
 *       we have to use sockaddr_storage to copy socket addres sockaddr_in6.
 *       In lwip size (alligment and padding) of the sockaddr is different than sockaddr_in6,
 *       so we can not copy from one to another.
 * **/
void copy_sockaddr_in6_to_sockaddr_storage(const struct sockaddr_in6 *src,
                                           struct sockaddr_storage *dest);

/**
 * @brief Safe cast sockaddr_in to sockaddr_storage
 * @param src pointer to source address
 * @param dest pointer to desination address
 * @return non
 * **/ 
void copy_sockaddr_in_to_sockaddr_storage(const struct sockaddr_in *src,
                                          struct sockaddr_storage *dest);


#endif /* __NET_CORE_H */
