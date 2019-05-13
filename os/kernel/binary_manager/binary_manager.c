/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <tinyara/sched.h>
#include <tinyara/binary_manager.h>

#include "binary_manager.h"

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include "../../arch/arm/src/imxrt/imxrt_gpio.h"
#include "../../arch/arm/include/imxrt/imxrt105x_irq.h"
#include "../../arch/arm/src/imxrt/chip/imxrt105x_pinmux.h"

#define IOMUX_GOUT      (IOMUX_PULL_NONE | IOMUX_CMOS_OUTPUT | \
                         IOMUX_DRIVE_40OHM | IOMUX_SPEED_MEDIUM | \
                         IOMUX_SLEW_SLOW)

#define IOMUX_SW8       (IOMUX_SLEW_FAST | IOMUX_DRIVE_50OHM | \
		IOMUX_SPEED_MEDIUM | IOMUX_PULL_UP_100K | \
		_IOMUX_PULL_ENABLE)

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/* Index of 'kernel' data in binary table, bin_table. */
#define KERNEL_IDX               0

/* Kernel version (not implemented, it will be modified) */
#ifdef CONFIG_VERSION_STRING
#define KERNEL_VER               CONFIG_VERSION_STRING
#else
#define KERNEL_VER               "2.0"
#endif

/* Binary table, the first data [0] is for kernel. */
binmgr_bininfo_t bin_table[BINARY_COUNT];
int g_bin_count;
int g_loadparam_part;
mqd_t g_binmgr_mq_fd;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Get the number of user binaries */
int binary_manager_get_binary_count(void)
{
	return g_bin_count;
}

/* Register partitions : kernel, loadparam, user binaries */
void binary_manager_register_partition(int part_num, int part_type, char *name, int part_size)
{
	int bin_idx;

	if (part_num < 0 || part_size <= 0 || part_type < 0 || part_type >= BINMGR_PART_MAX) {
		bmdbg("ERROR: Invalid part info : num %d, type %d size %d\n", part_num, part_type, part_size);
		return;
	}

	/* Check partition type and register it */
	if (part_type == BINMGR_PART_LOADPARAM) {
		g_loadparam_part = part_num;
	} else if (part_type == BINMGR_PART_KERNEL) {
		if (BIN_PARTSIZE(KERNEL_IDX) > 0) {
			/* Already registered first kernel partition, register it as second partition. */
			BIN_PARTNUM(KERNEL_IDX, 1) = part_num;
		} else {
			BIN_USEIDX(KERNEL_IDX) = 0;
			BIN_PARTNUM(KERNEL_IDX, 0) = part_num;
			BIN_PARTNUM(KERNEL_IDX, 1) = -1;
			BIN_PARTSIZE(KERNEL_IDX) = part_size;
			strncpy(BIN_VER(KERNEL_IDX), KERNEL_VER, KERNEL_VER_MAX);
			strncpy(BIN_KERNEL_VER(KERNEL_IDX), KERNEL_VER, KERNEL_VER_MAX);
			strncpy(BIN_NAME(KERNEL_IDX), "kernel", BIN_NAME_MAX);
		}
		bmvdbg("[KERNEL] part num %d size %d\n", part_num, part_size);
	} else {
		/* Else, It is user binary partition. First, Find the partition in the list. */
		for (bin_idx = 1; bin_idx <= g_bin_count; bin_idx++) {
			/* Found in the list, then register it as second partition */
			if (!strncmp(BIN_NAME(bin_idx), name, strlen(name) + 1)) {
				BIN_PARTNUM(bin_idx, 1) = part_num;
				bmvdbg("[USER2 : %d] %s %d %d \n", bin_idx, BIN_NAME(bin_idx), BIN_PARTNUM(bin_idx, 0), BIN_PARTNUM(bin_idx, 1));
				return;
			}
		}
		/* No, Register it as a new user partition */
		g_bin_count++;
		BIN_ID(g_bin_count) = -1;
		BIN_PARTNUM(g_bin_count, 0) = part_num;
		BIN_PARTNUM(g_bin_count, 1) = -1;
		BIN_PARTSIZE(g_bin_count) = part_size;
		strncpy(BIN_NAME(g_bin_count), name, BIN_NAME_MAX);
		bmvdbg("[USER1 : %d] %s size %d %d \n", g_bin_count, BIN_NAME(g_bin_count), BIN_PARTSIZE(g_bin_count), BIN_PARTNUM(g_bin_count, 0));
	}
}

void handler(int signo)
{
	//dbg("handler!!");
	binary_manager_recovery(9);
}

/****************************************************************************
 * Main Function
 ****************************************************************************/
int binary_manager(int argc, char *argv[])
{
	int ret;
	int nbytes;
//	sigset_t sigset;
	char type_str[1];
	char data_str[1];
	char *loading_data[LOADTHD_ARGC + 1];
	binmgr_request_t request_msg;

	struct mq_attr attr;
	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_request_t);
	attr.mq_flags = 0;

	bmvdbg("Binary Manager STARTED\n");

	/* Unset all signals except for SIGKILL */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGKILL);
//	sigdelset(&sigset, 23);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);
/*
	struct sigaction act;

	//register signal handler
	act.sa_handler = (_sa_handler_t)handler;
	act.sa_flags = 0;

	ret = sigaction(23, &act, NULL);
	if (ret == (int)SIG_ERR) {
		printf("sigaction Failed\n");
		return -1;
	}
*/
	/* Create binary manager message queue */
	g_binmgr_mq_fd = mq_open(BINMGR_REQUEST_MQ, O_RDWR | O_CREAT, 0666, &attr);
	if (g_binmgr_mq_fd < 0) {
		bmdbg("Failed to open message queue\n");
		return 0;
	}

	/* Execute loading thread for load all binaries */
	loading_data[0] = itoa(LOADCMD_LOAD_ALL, type_str, 10);
	loading_data[1] = NULL;

	ret = binary_manager_loading(loading_data);
	if (ret <= 0) {
		bmdbg("Failed to create loading thread\n");
		goto binary_manager_exit;
	} else {
		bmvdbg("Loading thread with pid %d will load binaries!\n", ret);
	}

	while (1) {
		ret = ERROR;
		bmvdbg("Wait for message\n");

		nbytes = mq_receive(g_binmgr_mq_fd, (char *)&request_msg, sizeof(binmgr_request_t), NULL);
		if (nbytes <= 0) {
			bmdbg("receive ERROR %d, errno %d, retry!\n", nbytes, errno);
			continue;
		}

//		sched_lock();

		bmvdbg("Recevied Request msg : cmd = %d\n", request_msg);
		switch (request_msg.cmd) {
#ifdef CONFIG_BINMGR_RECOVERY
		case BINMGR_FAULT:
			{
				binary_manager_recovery(request_msg.requester_pid);
			}
			break;
#endif
		case BINMGR_GET_INFO:
			ret = binary_manager_get_info_with_name(request_msg.requester_pid, request_msg.bin_name);
			break;
		case BINMGR_GET_INFO_ALL:
			ret = binary_manager_get_info_all(request_msg.requester_pid);
			break;
		case BINMGR_RELOAD:
			memset(type_str, 0, 1);
			memset(data_str, 0, 1);
			loading_data[0] = itoa(LOADCMD_RELOAD, type_str, 10);
			loading_data[1] = itoa(request_msg.requester_pid, data_str, 10);
			loading_data[2] = request_msg.bin_name;
			loading_data[3] = NULL;
			ret = binary_manager_loading(loading_data);
			break;

		default:
			break;
		}

//		sched_unlock();
	}

binary_manager_exit:
	bmvdbg("Recovery Manager EXITED\n");

	mq_close(g_binmgr_mq_fd);
	mq_unlink(BINMGR_REQUEST_MQ);

	return 0;
}
