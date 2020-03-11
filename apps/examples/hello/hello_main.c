/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <sys/types.h>
#include <tinyara/irq.h>

/* Define the time interval in seconds. This can be changed with a -D compiler option.  */

#ifndef TM_TEST_DURATION
#define TM_TEST_DURATION    30
#endif

int th0;
int th1;
int th2;
int th3;
int th4;
int th5;

/* Define the counters used in the demo application...  */

volatile unsigned long thread_0_counter;
volatile unsigned long thread_1_counter;
volatile unsigned long thread_2_counter;
volatile unsigned long thread_3_counter;
volatile unsigned long thread_4_counter;

/* Define the first preemptive thread.  */
void thread_0_entry(void)
{

	while(1)
	{

		/* Resume thread 1.  */
//		tm_thread_resume(1);
		fin_notify(th1, 0);

		/* We won't get back here until threads 1, 2, 3, and 4 all execute and
		   self-suspend.  */

		/* Increment this thread's counter.  */
		thread_0_counter++;
	}
}

/* Define the second preemptive thread.  */
void thread_1_entry(void)
{

	while(1)
	{

		/* Resume thread 2.  */
//		tm_thread_resume(2);
		fin_notify(th2, 0);

		/* We won't get back here until threads 2, 3, and 4 all execute and
		   self-suspend.  */

		/* Increment this thread's counter.  */
		thread_1_counter++;

		/* Suspend self!  */
//		tm_thread_suspend(1);
		fin_wait();
	}
}

/* Define the third preemptive thread.  */
void thread_2_entry(void)
{

	while(1)
	{

		/* Resume thread 3.  */
//		tm_thread_resume(3);
		fin_notify(th3, 0);

		/* We won't get back here until threads 3 and 4 execute and
		   self-suspend.  */

		/* Increment this thread's counter.  */
		thread_2_counter++;

		/* Suspend self!  */
//		tm_thread_suspend(2);
		fin_wait();
	}
}


/* Define the fourth preemptive thread.  */
void thread_3_entry(void)
{

	while(1)
	{

		/* Resume thread 4.  */
//		tm_thread_resume(4);
		fin_notify(th4, 0);

		/* We won't get back here until thread 4 executes and
		   self-suspends.  */

		/* Increment this thread's counter.  */
		thread_3_counter++;

		/* Suspend self!  */
//		tm_thread_suspend(3);
		fin_wait();
	}
}


/* Define the fifth preemptive thread.  */
void thread_4_entry(void)
{

	while(1)
	{

		/* Increment this thread's counter.  */
		thread_4_counter++;

		/* Self suspend thread 4.  */
//		tm_thread_suspend(4);
		fin_wait();
	}
}


/* Define the preemptive test reporting thread.  */
void thread_report(void)
{

	unsigned long total;
	unsigned long relative_time;
	unsigned long last_total;
	unsigned long average;


	/* Initialize the last total.  */
	last_total =  0;

	/* Initialize the relative time.  */
	relative_time =  0;

//	while(1)
//	{

		/* Sleep to allow the test to run.  */
//		tm_thread_sleep(TM_TEST_DURATION);
		sleep(TM_TEST_DURATION);

		/* Increment the relative time.  */
		relative_time =  relative_time + TM_TEST_DURATION;

		/* Print results to the stdio window.  */
		printf("**** Thread-Metric Preemptive Scheduling Test **** Relative Time: %lu\n", relative_time);

		/* Calculate the total of all the counters.  */
		total =  thread_0_counter + thread_1_counter + thread_2_counter
					+ thread_3_counter + thread_4_counter;

		/* Calculate the average of all the counters.  */
		average =  total/5;

		printf("Total : %lu , Avg : %lu\n", total, average);
		printf("th0 : %lu\n", thread_0_counter);
		printf("th1 : %lu\n", thread_1_counter);
		printf("th2 : %lu\n", thread_2_counter);
		printf("th3 : %lu\n", thread_3_counter);
		printf("th4 : %lu\n", thread_4_counter);

		/* See if there are any errors.  */
		if ((thread_0_counter < (average - 1)) || 
			(thread_0_counter > (average + 1)) ||
			(thread_1_counter < (average - 1)) || 
			(thread_1_counter > (average + 1)) ||
			(thread_2_counter < (average - 1)) || 
			(thread_2_counter > (average + 1)) ||
			(thread_3_counter < (average - 1)) || 
			(thread_3_counter > (average + 1)) ||
			(thread_4_counter < (average - 1)) || 
			(thread_4_counter > (average + 1)))
		{
			printf("ERROR: Invalid counter value(s). Preemptive counters should not be more that 1 different than the average!\n");
		}
		/* Show the time period total.  */
		printf("Time Period Total:  %lu\n\n", total - last_total);
		/* Save the last total.  */
		last_total =  total;
//	}
}

/* Define the preemptive scheduling test initialization.  */

void tm_preemptive_scheduling_initialize(void)
{

	/* Create the reporting thread. It will preempt the other 
	   threads and print out the test results.  */
	th5 = task_create("th5", 248, 1024, (main_t)thread_report, NULL);

	/* Create thread 4 at priority 6.  */
	th4 = task_create("th4", 249, 1024, (main_t)thread_4_entry, NULL);

	/* Create thread 3 at priority 7.  */
	th3 = task_create("th3", 250, 1024, (main_t)thread_3_entry, NULL);

	/* Create thread 2 at priority 8.  */
	th2 = task_create("th2", 251, 1024, (main_t)thread_2_entry, NULL);

	/* Create thread 1 at priority 9.  */
	th1 = task_create("th1", 252, 1024, (main_t)thread_1_entry, NULL);

	/* Create thread 0 at priority 10.  */
	th0 = task_create("th0", 253, 1024, (main_t)thread_0_entry, NULL);

	/* Resume just thread 0.  */
//	tm_thread_resume(0);
//	fin_notify(th0, 0);


//	tm_thread_resume(5);
//	fin_notify(th5, 0);
}
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Hello, World!!\n");

	tm_preemptive_scheduling_initialize();

	return 0;
}
