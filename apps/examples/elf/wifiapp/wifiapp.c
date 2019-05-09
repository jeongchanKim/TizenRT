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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sched.h>
#include <pthread.h>
#include <sys/prctl.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void gpio_handler(int signo)
{
	//write to pin22

	prctl(TC_WRITE_TO_PIN22, NULL);
}

int main(int argc, char **argv)
{
	int ret;
	struct sigaction act;
	sigset_t sigset;

	//register signal handler
	act.sa_handler = (_sa_handler_t)gpio_handler;
	act.sa_flags = 0;

	sigfillset(&sigset);
	sigdelset(&sigset, 1);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);

	ret = sigaction(1, &act, NULL);
	if (ret == (int)SIG_ERR) {
		printf("sigaction Failed\n");
		return -1;
	}

	while (1) {
		sleep(10);
		printf("[%d] WIFI ALIVE\n", getpid());
	}
	return 0;
}
