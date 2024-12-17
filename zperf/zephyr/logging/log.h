/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __LOG_H
#define __LOG_H

// #include "fsl_debug_console.h"

#define PRINTF printf

/**
 * @brief Register logging module
 * @note In NXP SDK enviroment there is no need to register module for logging
 * **/
#define LOG_MODULE_REGISTER(...)

/**
 * @brief Declare logging module
 * @note In NXP SDK enviroment there is no need to declare module for logging
 * **/
#define LOG_MODULE_DECLARE(...)

/**
 * @brief Network info log
 * **/
#define NET_INFO(fmt, ...)        PRINTF("INFO: [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * @brief Network error log
 * **/
#define NET_ERR(fmt, ...)         PRINTF("ERROR: [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * @brief Network warning log
 * **/
#define NET_WARN(fmt, ...)        PRINTF("WARNING: [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * @brief Network debug log
 * **/
#define NET_DBG(fmt, ...)         PRINTF("DEBUG: [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)


#endif /* __LOG_H */
