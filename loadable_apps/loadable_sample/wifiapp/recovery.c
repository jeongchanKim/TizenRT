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
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>

#if !defined(CONFIG_MPU_TEST_KERNEL_CODE_ADDR) || !defined(CONFIG_MPU_TEST_APP_ADDR)
#error "Address not defined for MPU test"
#endif

static void *assert_thread(void *index)
{
	int type;
	volatile uint32_t *addr;

	addr = (uint32_t *)CONFIG_MPU_TEST_KERNEL_CODE_ADDR;
	sleep(10);
	*addr = 0xdeadbeef;

	return 0;
}

///////////////////// threads

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

static int normal_task(int argc, char *argv[])
{
	while (1) {
		sleep(10);
	}
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

static void paper_test(void)
{
	int pid;
	int i;
	pthread_t thd;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

//wait mqueue
for (i = 0; i < 10; i++) {
	pid = task_create("mqwait", 100, 1024, normal_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}		
	//pthread_create(&thd, &attr, (pthread_startroutine_t)mq_wait_thread, (pthread_addr_t)NULL);
}

#if 0
	//wait sem (include tash task)
	for (i = 0; i < 3; i++) {
		pid = task_create("semwait", 100, 1024, sem_wait_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}		
		pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL); 	
		pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL); 	
	}
	pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL); 	
	pthread_create(&thd, &attr, (pthread_startroutine_t)sem_wait_thread, (pthread_addr_t)NULL); 	

	//wait mqueue
	for (i = 0; i < 4; i++) {
		pid = task_create("mqwait", 100, 1024, mq_wait_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}		
		pthread_create(&thd, &attr, (pthread_startroutine_t)mq_wait_thread, (pthread_addr_t)NULL);
		pthread_create(&thd, &attr, (pthread_startroutine_t)mq_wait_thread, (pthread_addr_t)NULL);
	}

	//wait signal (include main task)
	for (i = 0; i < 3; i++) {
		pid = task_create("sigwait", 100, 1024, normal_task, (FAR char *const *)NULL);
		if (pid < 0) {
			printf("task create FAIL\n");
			return 0;
		}
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)NULL);
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)NULL);
	}
	pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)NULL);
	pthread_create(&thd, &attr, (pthread_startroutine_t)assert_thread, (pthread_addr_t)NULL);
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void recovery_test(void)
{
	int pid;

	/*pid = task_create("mkchildren", 100, 1024, make_children_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return;
	}

	pid = task_create("normal", 100, 1024, normal_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}

	/*pid = task_create("assert_group_main", 100, 1024, assert_group_main_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}*/

	paper_test();

	printf("I'm RECOVERY main! create mkchildren task %d\n", pid);
}
