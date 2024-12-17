/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __TOOLCHAIN_H
#define __TOOLCHAIN_H

/**
 * @brief Macro that define variable as unused
 * **/
#define ARG_UNUSED(var)   (void)var

/**
 * @brief Unaligned access
 * **/
#define UNALIGNED_GET(p)                    \
__extension__ ({                            \
    struct  __attribute__((__packed__)) {   \
        __typeof__(*(p)) __v;               \
    } *__p = (__typeof__(__p)) (p);         \
    __p->__v;                               \
})

/**
 * @brief Explicitly indicate that a case in a switch statement is intentionally 
 *        designed to fall through to the next case
 * **/
#define __fallthrough   __attribute__ ((fallthrough))

#define Z_STRINGIFY(x)  #x
#define STRINGIFY(s)    Z_STRINGIFY(s)

#endif /* __TOOLCHAIN_H */
