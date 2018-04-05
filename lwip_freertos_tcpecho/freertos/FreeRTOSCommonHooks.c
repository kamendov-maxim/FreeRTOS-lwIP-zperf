/*
 * @brief Common FreeRTOS functions shared among platforms
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSCommonHooks.h"

#include "StackMacros.h"

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

//#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#if defined ( __ICCARM__ )
#define __WEAK__   __weak
#else
#define __WEAK__   __attribute__((weak))
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    taskENTER_CRITICAL();
    {
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
    exit(-1);
}


/* Delay for the specified number of milliSeconds */
void FreeRTOSDelay(uint32_t ms)
{
	TickType_t xDelayTime;

	xDelayTime = xTaskGetTickCount();
	vTaskDelayUntil(&xDelayTime, ms);
}

/* FreeRTOS malloc fail hook */
__WEAK__ void vApplicationMallocFailedHook(void)
{
//	taskDISABLE_INTERRUPTS();
//	__BKPT(0x01);
//	for (;; ) {}
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    vAssertCalled( __LINE__, __FILE__ );
}

/* FreeRTOS application idle hook */
__WEAK__ void vApplicationIdleHook(void)
{
	/* Best to sleep here until next systick */
//	__WFI();
}

/* FreeRTOS stack overflow hook */
//__WEAK__ void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
//{
//	(void) pxTask;
//	(void) pcTaskName;

	/* Run time stack overflow checking is performed if
	   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	   function is called if a stack overflow is detected. */
//	taskDISABLE_INTERRUPTS();
//	__BKPT(0x02);
//	for (;; ) {}
//}


#if 0
void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */

    /* Call the periodic timer test, which tests the timer API functions that
    can be called from an ISR. */
    #if( configUSE_PREEMPTION != 0 )
    {
        /* Only created when preemption is used. */
        vTimerPeriodicISRTests();
    }
    #endif

    /* Call the periodic queue overwrite from ISR demo. */
    vQueueOverwritePeriodicISRDemo();

    /* Write to a queue that is in use as part of the queue set demo to
    demonstrate using queue sets from an ISR. */
    vQueueSetAccessQueueSetFromISR();
    vQueueSetPollingInterruptAccess();

    /* Exercise event groups from interrupts. */
    vPeriodicEventGroupsProcessing();

    /* Exercise giving mutexes from an interrupt. */
    vInterruptSemaphorePeriodicTest();

    /* Exercise using task notifications from an interrupt. */
    xNotifyTaskFromISR();
}
#else
void vApplicationTickHook( void ) { }
#endif

#ifdef __CC_ARM
#ifndef EXTRA_HEAP_SZ
#define EXTRA_HEAP_SZ 0x6000
#endif
static uint32_t extra_heap[EXTRA_HEAP_SZ / sizeof(uint32_t)];
__attribute__((used)) unsigned __user_heap_extend(int var0, void **base, unsigned requested_size)
{
	if (requested_size > EXTRA_HEAP_SZ)
		return 0;

	*base = (void *) extra_heap;
	return sizeof(extra_heap);
}
#endif






