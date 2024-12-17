/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __INIT_H
#define __INIT_H

/**
 * @brief Initialization level
 * @note This macro has no effect in NXP enviroment.
 *       Initialization is done manually by calling function in application code.
 * **/
#define APPLICATION

/**
 * @brief Register an initialization function
 * @note This macro has no effect in NXP enviroment.
 *       Initialization is done manually by calling function in application code.
 * **/
#define SYS_INIT(function, level, prio)


#endif /* __INIT_H */   