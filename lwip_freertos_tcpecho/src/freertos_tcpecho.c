/*
 * @brief LWIP FreeRTOS TCP Echo example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
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

#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/tapif.h"
#include "netif/etharp.h"

#include "arch/sys_arch.h"
#include "apps/tcpecho_raw/tcpecho_raw.h"

#include "FreeRTOS.h"
#include "task.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* NETIF data */
static struct netif lpc_netif;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

void msDelay(uint32_t ms);

/* Sets up system hardware */
static void prvSetupHardware(void)
{
  /* Nothing to actually setup when running in the simulator. */
  return;
}

/* Callback for TCPIP thread to indicate TCPIP init is done */
static void tcpip_init_done_signal(void *arg)
{
  /* Tell main thread TCP/IP init is done */
  *(s32_t *) arg = 1;
}

/* LWIP kickoff and PHY link monitor thread */
static void vSetupIFTask(void *pvParameters)
{
  ip_addr_t ipaddr, netmask, gw;
  volatile s32_t tcpipdone = 0;
  uint32_t physts;
  static int prt_ip = 0;

  /* Wait until the TCP/IP thread is finished before
     continuing or wierd things may happen */
  LWIP_DEBUGF(LWIP_DBG_ON, ("Waiting for TCPIP thread to initialize...\n"));
  tcpip_init(tcpip_init_done_signal, (volatile void *) &tcpipdone);
  while (!tcpipdone) {
    msDelay(1);
  }

  LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP TCP echo server...\n"));

  /* Static IP assignment */
  IP4_ADDR(&gw, 10, 1, 10, 1);
  IP4_ADDR(&ipaddr, 10, 1, 10, 234);
  IP4_ADDR(&netmask, 255, 255, 255, 0);

  /* Add netif interface for lpc17xx_8x */
  if (!netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, tapif_init, ethernet_input)) {
    LWIP_ASSERT("Net interface failed to initialize\r\n", 0);
  }
  netif_set_default(&lpc_netif);
  netif_set_up(&lpc_netif);

  /* Initialize and start application */
  tcpecho_raw_init();

  static char tmp_buff[16];
  printf("IP_ADDR    : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.ip_addr, tmp_buff, 16));
  printf("NET_MASK   : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.netmask, tmp_buff, 16));
  printf("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.gw, tmp_buff, 16));

  while(1) {
    /* The minimal example located in the unix port in lwip-contrib uses
       tapif_select, but since this example is running under FreeRTOS, and
       extra thread (task) is created to accept the packets. */
#if 0
    tapif_select(&lpc_netif);
#endif
    sys_check_timeouts();
  }
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	MilliSecond delay function based on FreeRTOS
 * @param	ms	: Number of milliSeconds to delay
 * @return	Nothing
 * Needed for some functions, do not use prior to FreeRTOS running
 */
void msDelay(uint32_t ms)
{
	vTaskDelay((configTICK_RATE_HZ * ms) / 1000);
}

/**
 * @brief	main routine for example_lwip_tcpecho_freertos_18xx43xx
 * @return	Function should not exit
 */
int main(void)
{
  prvSetupHardware();

  /* FIXME: Implement command line options to configure the IP address. */

  /* Add another thread for initializing physical interface. This
     is delayed from the main LWIP initialization. */
  xTaskCreate(vSetupIFTask, "SetupIFx",
    configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
    (xTaskHandle *) NULL);

  /* Start the scheduler */
  vTaskStartScheduler();

  /* Should never arrive here */
  return 1;
}

