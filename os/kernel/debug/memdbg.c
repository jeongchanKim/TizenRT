/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/***************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>

/***************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

extern const uint32_t g_idle_topstack;

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: display_memory_information
 *
 * Description:
 *   This function prints the memory information for debug such as
 *   the RAM configuration, Idle stack configuration, Heap configuration.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void display_memory_information(void)
{
	/* Print RAM configuration */

	mllvdbg("-------------------RAM Configuration--------------------\n");
#ifdef CONFIG_SRAM_START_ADDR
	mllvdbg("Internal RAM   : start addr = 0x%x size = %u\n", CONFIG_SRAM_START_ADDR, (CONFIG_KSRAM_SIZE + CONFIG_USRAM_SIZE));
#if (defined(CONFIG_KSRAM_SIZE) && defined(CONFIG_RAM_KREGIONx_START))
	mllvdbg("Kernel   RAM   : start addr = 0x%x size = %u\n", kregionx_start[0], CONFIG_KSRAM_SIZE);
#endif
#if (defined(CONFIG_USRAM_SIZE) && defined(CONFIG_RAM_REGIONx_START))
	mllvdbg("User     RAM   : start addr = 0x%x size = %u\n", regionx_start[0], CONFIG_USRAM_SIZE);
#endif
#endif

	/* Print Idle Stack configuration */

	mllvdbg("Idle stack     : base addr = 0x%x size = %u\n", g_idle_topstack, CONFIG_IDLETHREAD_STACKSIZE);
	mllvdbg("-------------------RAM Configuration--------------------\n");
}