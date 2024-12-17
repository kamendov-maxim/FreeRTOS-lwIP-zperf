/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdio.h>

#include "thread.h"

/** see header **/
void k_sem_init(struct k_sem * sem,
                int initial_count,
                int max_count)
{
    assert(sem != NULL);
    sem->semaphore_handle = xSemaphoreCreateCounting(max_count, initial_count);
    assert(sem->semaphore_handle != NULL);
}

/** see header **/
void k_sem_take(struct k_sem * sem,
                const TickType_t ticks)
{
    assert(sem != NULL);
    xSemaphoreTake(sem->semaphore_handle, ticks);
}

/** see header **/
void k_sem_give(struct k_sem * sem)
{
    assert(sem != NULL);
    xSemaphoreGive(sem->semaphore_handle);
}

/** see header **/
void k_thread_create(struct k_thread * thread_struct,
                     StackType_t * thread_stack,
                     uint32_t size_of_stack,
                     thread_function thread_function,
                     void * args1,
                     void * args2,
                     void * args3,
                     int prio,
                     int flags,
                     int delay)
{
    assert(thread_struct != NULL);

    (void)thread_stack;
    (void)flags;
    (void)delay;
    (void)args2;
    (void)args3;

    if (xTaskCreate(thread_function,
                    thread_struct->name,
                    size_of_stack,
                    args1,
                    prio,
                    &thread_struct->task_hanble) != pdPASS)
    {
        assert(false);
    }
}

/** see header **/
void k_thread_name_set(struct k_thread * thread_struct,
                       const char * thread_name)
{
    assert(thread_struct != NULL);
    assert(thread_name != NULL);

    thread_struct->name = thread_name;
}
