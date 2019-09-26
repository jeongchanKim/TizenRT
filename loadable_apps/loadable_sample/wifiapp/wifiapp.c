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
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#ifdef CONFIG_BINARY_MANAGER
#include <binary_manager/binary_manager.h>
#endif

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/timer.h>

#include "wifiapp_internal.h"

#include <messaging/messaging.h>

#define ITERATION 1

//#define JCKIM_MSG_FW 1
#define TEST_DATA_SIZE 1000
/****************************************************************************
 * Public Functions
 ****************************************************************************/
int receiver_task(void)
{
	int ret;
	struct mq_attr attr;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = TEST_DATA_SIZE + 1;
	attr.mq_flags = 0;

	printf("Receiver Started\n");

	int frt_fd;
	char path[_POSIX_PATH_MAX];
	struct timer_status_s after;

	snprintf(path, _POSIX_PATH_MAX, "/dev/timer%d", 0);
#ifdef JCKIM_MSG_FW
	msg_recv_buf_t recv_data;
	recv_data.buf = (char *)malloc(TEST_DATA_SIZE + 1);
	recv_data.buflen = TEST_DATA_SIZE + 1;
#else
	char *ret_first = (char *)malloc(TEST_DATA_SIZE + 1);
#endif

	frt_fd = open(path, O_RDONLY);

#ifdef JCKIM_MSG_FW
	for (int i=0;i<ITERATION;i++) {
		ret = messaging_recv_block("jckim", &recv_data);
		if (ret < 0) {
			printf("receiver) FAIL to recv : %d\n", get_errno());
		}
	}
#else
//	sleep(1);
	mqd_t mqdes = mq_open("jckim", O_RDONLY | O_CREAT, 0666, &attr);
	for (int i=0;i<ITERATION;i++) {
		ret = mq_receive(mqdes, ret_first, TEST_DATA_SIZE + 1, 0);
		if (ret < 0) {
			printf("receiver) FAIL to recv : %d\n", get_errno());
		}
	}
#endif
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
	sleep(1);

#ifndef JCKIM_MSG_FW
	printf("1-%s\n", ret_first);
#else
	printf("2-%s\n", (char *)recv_data.buf);
#endif

	printf("RECEIVER FINISHED!! %u\n", after.timeleft);
	return 0;
}

extern int preapp_start(int argc, char **argv);

#ifdef CONFIG_APP_BINARY_SEPARATION
int main(int argc, char **argv)
#else
int wifiapp_main(int argc, char **argv)
#endif
{
#if defined(CONFIG_SYSTEM_PREAPP_INIT) && defined(CONFIG_APP_BINARY_SEPARATION)
	preapp_start(argc, argv);
#endif

	printf("This is WIFI App\n");

	/////
	receiver_task();

	while (1) {
		sleep(10);
		printf("[%d] WIFI ALIVE\n", getpid());
	}
	return 0;
}
