/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __UTIL_H
#define __UTIL_H

/**
 * @brief Get a pointer to a structure containing the element.
 * **/
#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/**
 * @brief Check for macro definition in compiler-visible expressions
 *
 * This trick was pioneered in Linux as the config_enabled() macro. It
 * has the effect of taking a macro value that may be defined to "1"
 * or may not be defined at all and turning it into a literal
 * expression that can be handled by the C compiler instead of just
 * the preprocessor. It is often used with a @p CONFIG_FOO macro which
 * may be defined to 1 via Kconfig, or left undefined.
 *
 * That is, it works similarly to <tt>\#if defined(CONFIG_FOO)</tt>
 * except that its expansion is a C expression. Thus, much <tt>\#ifdef</tt>
 * usage can be replaced with equivalents like:
 *
 *     if (IS_ENABLED(CONFIG_FOO)) {
 *             do_something_with_foo
 *     }
 *
 * This is cleaner since the compiler can generate errors and warnings
 * for @p do_something_with_foo even when @p CONFIG_FOO is undefined.
 *
 * @param config_macro Macro to check
 * @return 1 if @p config_macro is defined to 1, 0 otherwise (including
 *         if @p config_macro is not defined)
 */

#define IS_ENABLED(config_macro) Z_IS_ENABLED1(config_macro)

/** @brief This is called from IS_ENABLED(), and sticks on a "_XXXX" prefix,
 *         it will now be "_XXXX1" if config_macro is "1", or just "_XXXX" if it's
 *         undefined.
 *              ENABLED:   Z_IS_ENABLED2(_XXXX1)
 *              DISABLED   Z_IS_ENABLED2(_XXXX)
 * **/
#define Z_IS_ENABLED1(config_macro) Z_IS_ENABLED2(_XXXX##config_macro)

/** 
 * @brief Here's the core trick, we map "_XXXX1" to "_YYYY," (i.e. a string
 *        with a trailing comma), so it has the effect of making this a
 *        two-argument tuple to the preprocessor only in the case where the
 *        value is defined to "1"
 *              ENABLED:    _YYYY,    <--- note comma!
 *              DISABLED:   _XXXX
 * **/
#define _XXXX1 _YYYY,

/**
 * @brief Then we append an extra argument to fool the gcc preprocessor into
 *        accepting it as a varargs macro.
 *                                        arg1   arg2  arg3
 *              ENABLED:   Z_IS_ENABLED3(_YYYY,    1,    0)
 *              DISABLED   Z_IS_ENABLED3(_XXXX 1,  0)
 */
#define Z_IS_ENABLED2(one_or_two_args) Z_IS_ENABLED3(one_or_two_args 1, 0)

/**
 * @brief And our second argument is thus now cooked to be 1 in the case
 *        where the value is defined to 1, and 0 if not:
 */
#define Z_IS_ENABLED3(ignore_this, val, ...) val

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))


#endif /* __UTIL_H */
