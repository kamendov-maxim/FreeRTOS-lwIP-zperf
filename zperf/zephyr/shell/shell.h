/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SHELL_H
#define __SHELL_H

typedef void * 	shell_handle_t;

// #include "fsl_shell.h"
#include "shell_bt.h"

/**
 * @brief Initializes a shell command.
 * **/
#define SHELL_CMD(command, cmdList, descriptor, func) \
					    SHELL_CMD_ARG(command, cmdList, descriptor, func, 0, 0)

/**
 * @brief Enum defining type of colors for shell
 * **/
enum shell_vt100_color
{
	SHELL_VT100_COLOR_RED,
	SHELL_VT100_COLOR_GREEN,
	SHELL_VT100_COLOR_YELLOW,
	SHELL_VT100_COLOR_DEFAULT,
	VT100_COLOR_END
};  

/**
 * @brief Terminal default text color for shell_fprintf function.
 */
#define SHELL_NORMAL	(SHELL_VT100_COLOR_DEFAULT)

/**
 * @brief Green text color for shell_fprintf function.
 */
#define SHELL_INFO	    (SHELL_VT100_COLOR_GREEN)

/**
 * @brief Yellow text color for shell_fprintf function.
 */
#define SHELL_WARNING	(SHELL_VT100_COLOR_YELLOW)

/**
 * @brief Red text color for shell_fprintf function.
 */
#define SHELL_ERROR   	(SHELL_VT100_COLOR_RED)


/**
 * @brief Shell funtion for printing values on terminal
 * @param shell pointer to shell (not used)
 * @param color type of color that charcters in terminal should have
 * @param fmt   formating string
 * @param ...   list of paramaters to print
 * @return non
 * **/
#define shell_fprintf(shell, color, fmt, ...) do { \
    switch (color) { \
        case SHELL_NORMAL: \
            shell_print(shell, fmt, ##__VA_ARGS__); \
            break; \
        case SHELL_INFO: \
            shell_info(shell, fmt, ##__VA_ARGS__); \
            break; \
        case SHELL_WARNING: \
            shell_warn(shell, fmt, ##__VA_ARGS__); \
            break; \
        case SHELL_ERROR: \
            shell_error(shell, fmt, ##__VA_ARGS__); \
            break; \
    } \
} while(0)


/**
 * @brief Shell task function
 * @return non
 * **/
void shell_task();


#endif /* __SHELL_H */
