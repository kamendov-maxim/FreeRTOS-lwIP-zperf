/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include "lwip/errno.h"

#include "config.h"
#include "toolchain.h"

#include "work.h"
#include "util.h"

/** 
 * @brief Unsigned intiger bit position
 * **/
#define BIT(n)                         (1UL << (n))   

/** 
 * @brief User mode thread
 * @note This has no meaning, becasue in the freeRTOS enviroment all tasks (threads) runs in one mode
 *       and there is no such a thing as user/kernel space for protecting specific areas of memory and functionality.
 * **/
#define K_USER                         (BIT(2))

/** 
 * @brief Inhereit permissions for thread 
 * @note This has no meaning, becasue in the freeRTOS enviroment all tasks (threads) runs in one mode
 *       and there is no such a thing as user/kernel space for protecting specific areas of memory and functionality.
 * **/
#define K_INHERIT_PERMS                (BIT(3))

/** 
 * @brief Number of microseconds per millisecond
 * */
#define USEC_PER_MSEC                  (1000U)

/** 
 * @brief Number of milliseconds per second 
 * */
#define MSEC_PER_SEC                   (1000U)

/** @brief Number of microseconds per second */
#define USEC_PER_SEC                   ((USEC_PER_MSEC) * (MSEC_PER_SEC))

/**
 * @brief Clamp value to given range
 * **/
#define CLAMP(val, low, high)          (((val) <= (low)) ? (low) : MIN(val, high))

/**
 * @brief Alias for timepoint
 * **/
typedef uint64_t k_timepoint_t;

/**
 * @brief Converts mili-seconds to ticks
 * **/
#define K_MSEC(ms)        pdMS_TO_TICKS(ms)

/**
 * @brief Converts mili-seconds to ticks
 * **/
#define K_TICKS(ms)       pdMS_TO_TICKS(ms)

/**
 * @brief Add ticks (timestamp) to current ticks
 * @param ticks ticks to add
 * @return Calculated ticks
 * **/
static inline k_timepoint_t sys_timepoint_calc(TickType_t ticks)
{
    return ((k_timepoint_t)k_uptime_ticks() + (k_timepoint_t)ticks);
}

/**
 * @brief Check if time has expired
 * @param end time to check if has expired
 * @return True - time has expired
 *         False - time has not expired
 * **/
static inline bool sys_timepoint_expired(k_timepoint_t end)
{
    return ((k_timepoint_t)k_uptime_ticks() >= end) ? true : false;
}

/**
 * @brief Converts ticks to us
 * @param ticks ticks to convert
 * @return us as unsigned 32bit value
 * **/
uint32_t k_ticks_to_us_ceil32(uint32_t ticks);

/**
 * @brief Converts us to ticks
 * @param us us to convert
 * @return ticks as unsigned 32bit value
 * **/
uint32_t k_us_to_ticks_ceil32(uint32_t us);

/**
 * @brief Converts ticks to ms
 * @param ticks ticks to convert
 * @return us as unsigned 32bit value
 * **/
uint32_t k_ticks_to_ms_ceil32(uint32_t ticks);

/**
 * @brief Converts ms to ticks
 * @param ms ms to convert
 * @return ticks as unsigned 32bit value
 * **/
uint32_t k_ms_to_ticks_ceil32(uint32_t ms);

/**
 * @brief Converts ticks to us
 * @param ticks ticks to convert
 * @return us as unsingned 64bit value
 * **/
uint64_t k_ticks_to_us_ceil64(uint64_t ticks);

/**
 * @brief Converts ms to ticks
 * @param ms ms to convert
 * @return ticks as unsigned 64bit value
 * **/
uint64_t k_ms_to_ticks_ceil64(uint64_t ms);

/**
 * @brief Converts ticks to us
 * @param ticks ticks to convert
 * @return us as unsingned 64bit value
 * **/
static inline uint64_t k_ticks_to_us_floor64(uint64_t ticks)
{
    return k_ticks_to_us_ceil64(ticks);
}

#endif /* __KERNEL_H */
