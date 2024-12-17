/*
 * Copyright (c) 2023 Oliver Sintaj
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "work.h"
//#include "atomic.h"

#include <assert.h>
#include <stdio.h>

/**
 * @brief Task entry function for receiving items pushed to work queue
 * @param args pointer to arguments passed during thread creation
 * @return non
 * **/
static void _work_task(void * args)
{
    assert(args != NULL);
    k_work_q * ptr_to_work_q = (k_work_q*)(args);

    for (;;)
    {
        struct k_work * work;
        if (xQueueReceive(ptr_to_work_q->queue,
                          &work, portMAX_DELAY) == pdPASS)
        {
//            Atomic_XOR_u32(&work->is_pending, WORK_ITEM_IS_PENDING_BIT_MASK);
            work->is_pending ^= WORK_ITEM_IS_PENDING_BIT_MASK;
            work->handler(work->context);
        }
    }
}

/** see header **/
void k_work_queue_init(k_work_q * work_queue)
{
    assert(work_queue != NULL);

    work_queue->queue = xQueueCreate(DEFAULT_QUEUE_SIZE, sizeof(struct k_work*));
    assert(work_queue->queue != NULL);
}

/** see header **/
void k_work_queue_start(k_work_q * work_queue,
                        StackType_t * thread_stack,
                        size_t stack_size,
                        int prio,
                        void * cfg)
{
    (void)cfg;
    (void)thread_stack;

    assert(work_queue != NULL);

    if (xTaskCreate(_work_task, "_work_task", stack_size,
                   (void*)work_queue, prio,
                    &work_queue->thread.task_hanble ) != pdPASS)
    {
        assert(false);
    }
}

/** see header **/
void k_work_init(struct k_work * work,
                 k_work_handler handler)
{
    assert(work != NULL);
    work->handler = handler;
}

/** see header **/
void k_work_submit_to_queue(k_work_q * work_queue,
                            struct k_work * work)
{
    assert(work_queue != NULL);
    assert(work != NULL);

    work->context = work;
//    Atomic_OR_u32(&work->is_pending, WORK_ITEM_IS_PENDING_BIT_MASK);
    work->is_pending |= WORK_ITEM_IS_PENDING_BIT_MASK;
    xQueueSend(work_queue->queue, (void*)&work, portMAX_DELAY);
}

/** see header **/
bool k_work_is_pending(struct k_work * work)
{
    assert(work != NULL);
//    return (Atomic_AND_u32(&work->is_pending, WORK_ITEM_IS_PENDING_BIT_MASK) > 0);
//    REALLY?
    return (work->is_pending & WORK_ITEM_IS_PENDING_BIT_MASK);
}
