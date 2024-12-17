/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __THREAD_H
#define __THREAD_H

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/** @brief Forward decleration for thread struct **/
struct k_thread;

/** @brief Forward decleration for semaphore struct **/
struct k_sem;

/** @brief Forward decleration for threat stack struct **/
struct thread_stack;

/**
 * @brief Return priority for preemtive thread
 * 
 * @note In freeRTOS enviroment this macro has no special meaning, becasue type of schedulling is configured
 *       globally using configUSE_PREEMPTION macro. So unlike in Zephyr OS, where by priority you can specify scheduling behaviour,
 *       in freeRTOS u done it by macro configUSE_PREEMPTION.
 * **/
#define K_PRIO_PREEMPT(value)          (value)

/**
 * @brief Return priority for cooperative thread
 * 
 * @note In freeRTOS enviroment this macro has no special meaning, becasue type of schedulling is configured
 *       globally using configUSE_PREEMPTION macro. So unlike in Zephyr OS, where by priority you can specify scheduling behaviour,
 *       in freeRTOS u done it by macro configUSE_PREEMPTION.
 * 
 *       In Zephyr OS, this macro would return only negative value (the smaller the number, the higher a priotiy - opposite behaviour to freeRTOS)
 *       and by this achive cooperative schedulling, by making thread running with highest priority and avoid interrption.
 * **/
#define K_PRIO_COOP(value)             (value)

/**
 * @brief Infinite ticks
 * **/
#define K_FOREVER                      (portMAX_DELAY) 

/**
 * @brief No wait or block funtion call
 * **/
#define K_NO_WAIT                      (0)

/**
 * @brief Declare and intialize counting semaphore
 * @note In Zephyr, this can also initialize semaphore, in freeRTOS enviroment
 *       this is not possible, so creating semaphore has to be done manully by k_sem_init
 * **/
#define K_SEM_DEFINE(name, initial_count, max_count)      \
    struct k_sem name = {  NULL,                          \
                           initial_count,                 \
                           max_count }                    \

/**
 * @brief Defines thread struct and initialize size
 * @note  In freeRTOS we do not have to allocate stack separatly, xTaskCreate will handle it
 *        if we use dynamic allocation for task. If static allocation would be used, this macro
 *        can be used. Currently this is due to compability with  original Zperf.
 * **/
#define K_THREAD_STACK_DEFINE(name, size) \
    StackType_t name[size]

/**
 * @brief Returns size of the thread stack
 * **/
#define K_THREAD_STACK_SIZEOF(thread_stack) \
    (sizeof(thread_stack) / sizeof(StackType_t))

/**
 * @brief Returns current tick since vTaskStartScheduler was called
 * 
 * @warning Return ticks as 32bits not as 64bit as k_uptime_ticks from zephyr
 * **/
#define k_uptime_ticks()  xTaskGetTickCount()

/**
 * @brief Yield the CPU to run another task of equal priority.
 * **/
#define k_yield()         taskYIELD()

/**
 * @brief Put current thread to sleep for specif amount of time
 * **/
#define k_sleep(delay)    vTaskDelay(delay)

/**
 * @brief Function handler for entry function in thread
 * **/
typedef void (*thread_function)(void*);

/**
 * @brief Struct of Task Control Block (TCB)
 * @note FreeRTOS does not use TCB ouside of freeRTOS API,
 *       so this is just for compatibilty with zperf
 * **/
struct k_thread
{
    /** @brief Task handler **/
    TaskHandle_t task_hanble;
    /** @brief Thread name **/
    const char * name;
};

/**
 * @brief Struct holding information about thread stack
 * @note  In freeRTOS we do not have to allocate stack separatly, xTaskCreate will handle it
 * **/
struct thread_stack
{
    /** @brief Size of the thread stack **/
    uint32_t stack_size;
};

/**
 * @brief Struct for semaphore
 * **/
struct k_sem
{
    /** @brief Semaphore handle **/
    SemaphoreHandle_t semaphore_handle;
    /** @brief Initial  semaphore count **/ 
    uint32_t initial_count;
    /** @brief Maximum permitted semaphore count **/ 
    uint32_t max_count;
};

/**
 * @brief Initialize semaphore
 * @param sem pointer to semaphores struct
 * @param initial_count initil semaphore count
 * @param max_count maximum permitted semaphore count
 * @return non
 * **/
void k_sem_init(struct k_sem * sem,
                int initial_count,
                int max_count);

/**
 * @brief Semaphore take function
 * @param sem pointer to semaphores struct
 * @param ticks The time in ticks to wait for the semaphore to become
 *              available
 * @return non
 * **/
void k_sem_take(struct k_sem * sem,
                const TickType_t ticks);

/**
 * @brief Semaphore release function
 * @param sem pointer to semaphore struct
 * @return non
 * **/
void k_sem_give(struct k_sem * sem);

/**
 * @brief Spawn new thread (using dynamic allocation) 
 * 
 * @param thread_struct pointer to thread (TCB) struct 
 * @param thread_stack thread stack (not used)
 * @param size_of_stack size of thread stack
 * @param thread_function thread entry function
 * @param args thread entry function arguments
 * @param args thread entry function arguments (not used)   
 * @param args thread entry function arguments (not used)
 * @param prio thread priority
 * @param args thread flags (not used)
 * @param args delay until thread should be passed to scheduler (not used)
 * @return non
 * 
 * @note Arguments which are not used have no meaningfull usage in freeRTOS enviroment
 *       and there are due compatibility. But if Task was created staticly, then
 *       does argments had meaning, for instance thread_stack.
 * **/
void k_thread_create(struct k_thread * thread_struct,
                     StackType_t * thread_stack,
                     uint32_t size_of_stack,
                     thread_function thread_function,
                     void * args1,
                     void * args2,
                     void * args3,
                     int prio,
                     int flags,
                     int delay);
                     
/**
 * @brief Sets name of the thread
 * @param thread_struct pointer to thread structy
 * @param thread_name name of the thread
 * @return non
 * **/          
void k_thread_name_set(struct k_thread * thread_struct,
                       const char * thread_name);


#endif /* __THREAD_H */ 