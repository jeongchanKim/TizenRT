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

#include <stdio.h>
#include <unistd.h>
#ifdef CONFIG_BINARY_MANAGER
#include <binary_manager/binary_manager.h>
#endif
#include "micomapp_internal.h"

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <tinyara/timer.h>

#include <messaging/messaging.h>

#define ITERATION 1

//#define JCKIM_MSG_FW 1
#define TEST_DATA_SIZE 1000
/****************************************************************************
 * Public Functions
 ****************************************************************************/
int sender_task(void)
{
	struct mq_attr attr;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = TEST_DATA_SIZE + 1;
	attr.mq_flags = 0;
	int ret;
	//10bytes
//	char *first_data = "0abcdefghi";

	//100bytes
//	char *first_data = "0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh";

	//1000 bytes
	char *first_data = "0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg";

	//10000 bytes
//	char *first_data = "0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdefg0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh0abcdefgh1abcdefgh2abcdefgh3abcdefgh4abcdefgh5abcdefgh6abcdefgh7abcdefgh8abcdefgh9abcdefgh10bcdefgh11abcdIwannagohome";

	printf("Sender Started %d\n", TEST_DATA_SIZE + 1);

	int frt_fd;
	char path[_POSIX_PATH_MAX];
	struct timer_status_s before;
	struct timer_status_s after;
	struct timer_status_s mid;
	struct timer_status_s openn;
	msg_send_data_t param;

	param.priority = 10;
	param.msg = first_data;

	param.msglen = TEST_DATA_SIZE + 1;

	snprintf(path, _POSIX_PATH_MAX, "/dev/timer%d", 0);

	frt_fd = open(path, O_RDONLY);

	ioctl(frt_fd, TCIOC_SETFREERUN, TRUE);
	ioctl(frt_fd, TCIOC_START, TRUE);
#ifdef JCKIM_MSG_FW
	sleep(2);
#else
	sleep(1);
#endif
	char *ret_first = (char *)malloc(TEST_DATA_SIZE + 1);

#ifdef JCKIM_MSG_FW
//	for(int i=0;i<ITERATION;i++) {
		ret = messaging_send("jckim", &param);
		if (ret != OK) {
			printf("sender) FAIL to send : %d\n", get_errno());
			return -1;
		}
//	}
#else
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&openn);
	mqd_t mqdes = mq_open("jckim", O_RDWR | O_CREAT, 0666, &attr); //O_WRONLY
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
//	for (int i=0;i<ITERATION;i++) {
		ret = mq_send(mqdes, first_data, TEST_DATA_SIZE + 1, 11);
		if (ret != OK) {
			printf("sender) FAIL to send : %d\n", get_errno());
			return -1;
		}
//	}
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&mid);
	ret = mq_receive(mqdes, ret_first, TEST_DATA_SIZE + 1, 0);
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
	if (ret < 0) {
		printf("receiver) FAIL to recv : %d\n", get_errno());
	}
	printf("!-%s\n", (char *)ret_first);
#endif

//	sleep(3);
	printf("SENDER FINISHED!!, %u\n", before.timeleft);
	printf("SENDER AFTER!!, %u, %u\n", after.timeleft, after.timeleft - before.timeleft);
	printf("SEND : %u\n", mid.timeleft - before.timeleft);
	printf("RECV : %u\n", after.timeleft - mid.timeleft);
	printf("open : %u\n", before.timeleft - openn.timeleft);

	ioctl(frt_fd, TCIOC_STOP, 0);
	return 0;
}

#ifdef CONFIG_APP_BINARY_SEPARATION
int main(int argc, char **argv)
#else
int micomapp_main(int argc, char **argv)
#endif
{
	printf("MICOM STARTED\n");

	sender_task();

	while (1) {
		sleep(10);
		printf("[%d] MICOM ALIVE\n", getpid());
	}

	return 0;

}
