/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "FreeRTOSConfig.h"
#include "lwipopts.h"

#if !defined(configUSE_PREEMPTION)
    #error "Missing configUSE_PREEMPTION in FreeRTOSConfig.h"
#endif /* configUSE_PREEMPTION */

/**
 * @brief If configUSE_PREEMPTION is set to zero than cooperative multitasking is configured
 * 
 * @note There is difference between scheduling in freeRTOS and Zephyr. Zephyr OS allows having two types of schedulling 
 *       during runtime of the application based on thread priority, using K_PRIO_COOP or K_PRIO_PREEMPT. FreeRTOS defines globally
 *       types of schedulling. Means that during application run-time there can be only one type of schedulling, cooperative or preemtive,
 *       unlike in Zephyr OS, where by priorities can be cooperative and preemtive scheduling achivied at once.
 * 
 *       In Zephyr OS CONFIG_NET_TC_THREAD_COOPERATIVE meant only, that some networking threads has been scheduled as cooperative.
 *       (Cannot be interupted by thread with higher priority). In freeRTOS enviroment, when this macro is active, all other threads
 *       are sheduled as cooperative.
 * **/
#if (configUSE_PREEMPTION == 0)
    #define CONFIG_NET_TC_THREAD_COOPERATIVE
#endif /* configUSE_PREEMPTION == 0 */

// /**
//  * @brief Compile time check for freeRTOS task static allocation. Zperf by default use static allocationf for
//  *        his tasks.
//  * **/
// #if (configSUPPORT_STATIC_ALLOCATION == 0)
//     #error "Zperf use static allocation for tasks. See your FreeRTOSConfig.h in your applicaiton and set configSUPPORT_STATIC_ALLOCATION to 1"
// #endif /* configSUPPORT_STATIC_ALLOCATION == 0 */

/**
 * @brief Enables IPv4 
 * **/
#define CONFIG_NET_IPV4                      LWIP_IPV4

/**
 * @brief Enables IPv6 
 * **/
#define CONFIG_NET_IPV6                      LWIP_IPV6

/**
 * @brief Enables that threads may be created or dropped down to user mode
 * 
 * @note This has no meaning, becasue in the freeRTOS enviroment all tasks (threads) runs in one mode
 *       and there is no such a thing as user/kernel space for protecting specific areas of memory and functionality.
 * **/
// #define CONFIG_USERSPACE                     0 

/**
 * @brief Enables shell module that provides network commands 
 * **/
#define CONFIG_NET_SHELL                     1

#if (CONFIG_NET_IPV6 > 0)
/**
 * @brief Defines device IPv6 address
 * **/
#define CONFIG_NET_CONFIG_MY_IPV6_ADDR       "2001:db8::1"

/**
 * @brief Defines host IPv6 address
 * **/
#define CONFIG_NET_CONFIG_PEER_IPV6_ADDR     "2001:db8::2"
#endif // CONFIG_NET_IPV6 > 0

#if (CONFIG_NET_IPV4 > 0)
/**
 * @brief Defines device IPv4 address
 * **/
#define CONFIG_NET_CONFIG_MY_IPV4_ADDR       "192.168.0.2"

/**
 * @brief Defines host IPv4 address
 * **/
#define CONFIG_NET_CONFIG_PEER_IPV4_ADDR     "192.168.0.1"
#endif // CONFIG_NET_IPV4 > 0

/**
 * @brief Define if UDP support is active
 * **/
#define CONFIG_NET_UDP                       LWIP_UDP

/**
 * @brief Define if TCP support is active
 * **/
#define CONFIG_NET_TCP                       LWIP_TCP

/**
 * @brief Enables debug prints (logs) for zperf
 * **/
// #define CONFIG_NET_ZPERF_LOG_LEVEL_DBG       0

/**
 * @brief Enables possibility to prioritize network traffic
 * 
 * @warning This functionality is not supported by lwip
 * **/
// #define CONFIG_NET_CONTEXT_PRIORITY          0

/**
 * @brief Enables possibility to prioritize network traffic
 * 
 * @warning This functionality is not supported by lwip
 * **/
// #define CONFIG_NET_ALLOW_ANY_PRIORITY        0

/**
 * @brief Highest possible application thread priority
 * **/
#define K_HIGHEST_APPLICATION_THREAD_PRIO    (configMAX_PRIORITIES - 1)

/**
 * @brief Lowest possible application thread priority
 * **/
#define K_LOWEST_APPLICATION_THREAD_PRIO     (tskIDLE_PRIORITY)

/**
 * @brief Priority of the work queue thread
 * 
 * @note In zperf sample app from Zephyr SDK is used by default K_HIGHEST_APPLICATION_THREAD_PRIO,
 *       which has lowest priority for sheduling, but in freeRTOS enviroment, the lowest numeric value,
 *       has lowest priority, so opposite behaviou to Zephyr, where highest numberic value has lowest priority 
 * **/
#define CONFIG_ZPERF_WORK_Q_THREAD_PRIORITY  (K_HIGHEST_APPLICATION_THREAD_PRIO)

/**
 * @brief Eneble configuration of ethernet
 * 
 * @warning This macro has no real effect in NXP lwip enviroment, unlike in Zephyr, where configurarion of ethernet
 *          is enabled by this macro, in lwip ethernet configuration has to be done manaully in application code.
 * **/
#define CONFIG_NET_L2_ETHERNET               1

/**
 * @brief Defines maximal size for socket
 * **/
#define CONFIG_NET_ZPERF_MAX_PACKET_SIZE     (1024)

/**
 * @brief Enebles POSIX acrhitecture to be able to run app on desktop enviroment
 * 
 * @warning Porting to POSIX architacture is not able in freeRTOS enviroment.
 * **/
// #define CONFIG_ARCH_POSIX                    0

/**
 * @brief Defines maximal count of zperf sessions
 * **/
#define CONFIG_NET_ZPERF_MAX_SESSIONS        (4)

/**
 * @brief Defines stack size for work queue
 * **/
#define CONFIG_ZPERF_WORK_Q_STACK_SIZE       (2048)

/**
 * @brief Priority of kernel initiazation function
 * @note Has no meaning in NXP envroment. NXP does not support initialization functions.
 * **/
#define CONFIG_KERNEL_INIT_PRIORITY_DEFAULT

#endif /* __CONFIG_H */
