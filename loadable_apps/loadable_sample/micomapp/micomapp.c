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
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <semaphore.h>
#ifdef CONFIG_BINARY_MANAGER
#include <binary_manager/binary_manager.h>
#endif
#include "micomapp_internal.h"
#include <sys/prctl.h>

///////////////////// threads
static void *assert_thread(void *index)
{
	int type;
	volatile uint32_t *addr;

	addr = (uint32_t *)CONFIG_MPU_TEST_KERNEL_CODE_ADDR;
	sleep(5);
	*addr = 0xdeadbeef;

	return 0;
}

static void *normal_thread(void *index)
{
	while (1) {
		sleep(10);
	};
}

static void *sem_wait_thread(void *index)
{
	sem_t test_sem;

	sem_init(&test_sem, 0, 0);
	sem_wait(&test_sem);

}

static void *mq_wait_thread(void *index)
{
	int nbytes;
	mqd_t mqfd;
	char mq_name[32];
	char data[32];

	struct mq_attr attr;
	attr.mq_maxmsg = 32;
	attr.mq_msgsize = 16;
	attr.mq_flags = 0;

	memset(mq_name, 0, 32);

	sprintf(mq_name, "mymqueue%d", getpid());
	
	mqfd = mq_open(mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (mqfd < 0) {
		printf("Failed to open message queue\n");
		return 0;
	}

	nbytes = mq_receive(mqfd, (char *)data, 32, NULL);
	if (nbytes <= 0) {
		printf("Receive ERROR %d, errno %d, retry!\n", nbytes, errno);
	}
}

/////////////////////tasks

static int assert_task(int argc, char *argv[])
{
	int type;
	volatile uint32_t *addr;

	addr = (uint32_t *)CONFIG_MPU_TEST_KERNEL_CODE_ADDR;
	sleep(3);
	*addr = 0xdeadbeef;

	return 0;

}

static int normal_task(int argc, char *argv[])
{
	while (1) {
		sleep(10);
	};
}

static int sem_wait_task(int argc, char *argv[])
{
	sem_t test_sem;

	sem_init(&test_sem, 0, 0);
	sem_wait(&test_sem);
}

static int mq_wait_task(int argc, char *argv[])
{
	int nbytes;
	mqd_t mqfd;
	char mq_name[32];
	char data[32];
	struct timespec ts;

	struct mq_attr attr;
	attr.mq_maxmsg = 32;
	attr.mq_msgsize = 16;
	attr.mq_flags = 0;

	memset(mq_name, 0, 32);

	sprintf(mq_name, "mymqueue%d", getpid());
	
	mqfd = mq_open(mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (mqfd < 0) {
		printf("Failed to open message queue\n");
		return 0;
	}

	int status = clock_gettime(CLOCK_REALTIME, &ts);
	if (status != 0) {
		printf("sender_thread: ERROR clock_gettime failed\n");
	}
	ts.tv_sec += 5;

	//nbytes = mq_receive(mqfd, (char *)data, 32, NULL);
	nbytes = mq_timedreceive(mqfd, (char *)data, 32, NULL, &ts);
	if (nbytes <= 0) {
		printf("Receive ERROR %d, errno %d, retry!\n", nbytes, errno);
	}
 
}

static void paper_test(void)
{
	int pid;
	int i;
	pthread_t thd;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	attr.priority = 220;

	for (i = 0; i < 8; i++) {
		pid = task_create("sem_wait", 220, 1024, sem_wait_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}
	}

	//pthread_create(&thd, &attr, (pthread_startroutine_t)assert_thread, (pthread_addr_t)NULL);

	//wait sem
#if 0
	for (i = 0; i < 2; i++) {
		pid = task_create("semwait", 220, 1024, sem_wait_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}
		pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL);
		pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL);
	}

	//wait mqueue
	for (i = 0; i < 2; i++) {
		pid = task_create("mqwait", 220, 1024, mq_wait_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}
		pthread_create(&thd, &attr, (pthread_startroutine_t)mq_wait_thread, (pthread_addr_t)NULL);
		pthread_create(&thd, &attr, (pthread_startroutine_t)mq_wait_thread, (pthread_addr_t)NULL);
	}

	//wait signal (include main)
	pid = task_create("sigwait", 220, 1024, mq_wait_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}
	for (i = 0; i < 2; i++) {
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)NULL);
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)NULL);
	}
#endif
}

extern int preapp_start(int argc, char **argv);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
int main(int argc, char **argv)
#else
int micomapp_main(int argc, char **argv)
#endif
{

	//prctl(TC_GPIO_PIN20_CONFIG);

	//prctl(TC_GPIO_PIN27_TRUE);
	//prctl(TC_GPIO_PIN27_FALSE);
	
#if defined(CONFIG_SYSTEM_PREAPP_INIT) && defined(CONFIG_APP_BINARY_SEPARATION)
	//preapp_start(argc, argv);
#endif

#if 0
	int ret;
	
#ifdef CONFIG_EXAMPLES_MICOM_TIMER_TEST
	char *timer_args[TIMER_ARG_NUM];

	ret = alloc_timer_args(timer_args);
	if (ret != OK) {
		printf("TIMER TEST FAIL : out of memory.\n");
		return ERROR;
	}

	timer_example_main(TIMER_ARG_NUM, timer_args);

	free_timer_args(timer_args);
#else /* CONFIG_EXAMPLES_MICOM_TIMER_TEST */
#ifdef CONFIG_BINARY_MANAGER
	ret = binary_manager_notify_binary_started();
	if (ret < 0) {
		printf("MICOM notify 'START' state FAIL\n");
	}
#endif

	int count;
	pthread_t thd;
	pthread_attr_t attr;

	printf("[%d] assert_group_main_task \n", getpid());

	pthread_attr_init(&attr);

	printf("[%d] normal_task \n", getpid());

	for (count = 0; count < 3; count++) {
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)count);
	}

#ifdef CONFIG_EXAMPLES_MESSAGING_TEST
	messaging_test();
#endif
#endif

#endif /* CONFIG_EXAMPLES_MICOM_TIMER_TEST */
//	paper_test();

	volatile uint32_t *addr;

	addr = (uint32_t *)CONFIG_MPU_TEST_KERNEL_CODE_ADDR;
	sleep(3);
	*addr = 0xdeadbeef;

	while (1) {
		sleep(10);
		//printf("[%d] MICOM ALIVE\n", getpid());
	}

	return 0;

}
