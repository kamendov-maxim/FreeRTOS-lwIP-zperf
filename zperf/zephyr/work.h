/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __WORK_H
#define __WORK_H

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "thread.h"

/** @brief Forward decleration for work item struct **/
struct k_work;

/** @brief Forward decleration for work queue struct **/
struct k_work_q_struct;

/**
 * @brief Default size of queue items
 * **/
#define DEFAULT_QUEUE_SIZE             (10)

/**
 * @brief Bit position of is_pending flag for work queue item
 * **/
#define WORK_ITEM_IS_PENDING_BIT       (0U)

/**
 * @brief Bit-field for is_pending flag for work queue item
 * **/
#define WORK_ITEM_IS_PENDING_BIT_MASK  (1UL << (WORK_ITEM_IS_PENDING_BIT))

/**
 * @brief Function handler for work queue item
 * **/
typedef void (*k_work_handler)(struct k_work*);

/**
 * @brief Alias for work queue
 * **/
typedef struct k_work_q_struct k_work_q;

/**
 * @brief Struct of work item
 * **/
struct k_work
{
    /** @brief Function handler **/
    k_work_handler handler;

    /** @brief Pointer to context passed to the handler **/
    struct k_work * context;
    
    /** 
     * @brief Flag inticates if item is pending in queue 
     * @warning Should be handled as atomic
     * **/
    volatile uint32_t is_pending;
};

/**
 * @brief Struct for work queue
 * **/
struct k_work_q_struct
{
    /** @brief Task control block instance **/
    struct k_thread thread;
        
    /** @brief Queue handle **/
    QueueHandle_t queue;
};

/**
 * @brief Initilization of work queue (Creation of queue)
 * @param work_queue pointer to work queue
 * @return non
 * **/
void k_work_queue_init(k_work_q * work_queue);

/**
 * @brief Initilization of work item - assigning handler for work item
 * @param work pointer to work item
 * @param handler hanlder to assign to work item
 * @return non
 * **/
void k_work_init(struct k_work * work,
                 k_work_handler handler);

/**
 * @brief Queue start (receiving thread is spawned)
 * @param work_queue pointer to work queue
 * @param thread_stack copy of stack instance (not used)
 * @param stack_size stack size of receiving thread
 * @param prio priority of receiving thread
 * @param cfg pointer to work queue configuration (not used)
 * @return non
 * 
 * @note Arguments which are not used have no meaningfull usage in freeRTOS enviroment
 * **/
void k_work_queue_start(k_work_q * work_queue,
                        StackType_t * thread_stack,
                        size_t stack_size,
                        int prio,
                        void * cfg);

/**
 * @brief Submit/send work item to queue
 * @param work_queue pointer to the work queue
 * @param work pointer to work item
 * @return non
 * **/
void k_work_submit_to_queue(k_work_q * work_queue,
                            struct k_work * work);

/**
 * @brief Check if work item is in pending state
 * @param work pointer to work item
 * @return True - work item is in pending state
 *         False - work item is not in pending state
 * **/
bool k_work_is_pending(struct k_work * work);


#endif /* __WORK_H */