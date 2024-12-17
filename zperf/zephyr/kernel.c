/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "kernel.h"

/** @brief Tick rate in hertz **/
static const uint64_t _tick_rate_hz = configTICK_RATE_HZ;

/** see header **/
uint32_t k_ticks_to_us_ceil32(uint32_t ticks)
{
    // To avoid floating point arithmetic, rearrange it to:
    // (ticks * 1000000) / tick_rate_hz
    uint32_t us = (uint32_t)(((uint64_t)ticks * (uint64_t)USEC_PER_SEC) / _tick_rate_hz);
    return us;
}

/** see header **/
uint32_t k_us_to_ticks_ceil32(uint32_t us)
{
    uint32_t ticks = (uint32_t)(((uint64_t)us * _tick_rate_hz) / (uint64_t)USEC_PER_SEC);
    return ticks;
}

/** see header **/
uint32_t k_ticks_to_ms_ceil32(uint32_t ticks)
{
    // To avoid floating point arithmetic, rearrange it to:
    // (ticks * 1000) / tick_rate_hz
    uint32_t ms = (uint32_t)(((uint64_t)ticks * (uint64_t)USEC_PER_MSEC) / _tick_rate_hz);
    return ms;
}

/** see header **/
uint32_t k_ms_to_ticks_ceil32(uint32_t ms)
{
    uint32_t ticks = (uint32_t)(((uint64_t)ms * _tick_rate_hz) / (uint64_t)USEC_PER_MSEC);
    return ticks;
}

/** see header **/
uint64_t k_ticks_to_us_ceil64(uint64_t ticks)
{
    // To avoid floating point arithmetic, rearrange it to:
    // (ticks * 1000000) / tick_rate_hz
    uint64_t us = (ticks * (uint64_t)USEC_PER_SEC) / _tick_rate_hz;
    return us;
}

/** see header **/
uint64_t k_ms_to_ticks_ceil64(uint64_t ms)
{
    uint64_t ticks = (ms * _tick_rate_hz) / (uint64_t)USEC_PER_MSEC;
    return ticks;
}
