#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/random.h>

#include "rtai.h"
#include "rtai_sched.h"
#include "rtai_fifos.h"
#include "rtai_sem.h"
#include "rtai_mbx.h"

#include "values.h"
#include "calib.h"

MODULE_LICENSE("GPL"); 

int retval;
int log_len;
long long s;


int rand(int min, int max){
	unsigned random_val;
	get_random_bytes(&random_val, sizeof(unsigned));
	random_val = min + (random_val % (max + 1 - min));
	if (random_val > max) random_val = max;
	return random_val;
}

/* Variables for task creation */ 
RT_TASK tsk1, tsk2, tsk3;

SEM mux1, mux2;

void send_log(int task_id, int trans_id){
	log l;
	l.task_id = task_id;
	l.transition_id = trans_id;
	l.time = rt_get_cpu_time_ns();
	rtf_put(FIFO_ID_LOG, &l, sizeof(l) );	
}


void tsk1_job(long arg){
	while(1){
		//rt_printk("Task 3 resumed");
		send_log(1, 10);
		
		rt_sem_wait(&mux1);
		send_log(1, 11);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(1, 12);
		
		rt_sem_signal(&mux1);
		
		busy_sleep(rand(5, 10) * MILLISEC);
		send_log(1, 13);
		
		rt_sem_wait(&mux2);
		send_log(1, 14);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(1, 15);
		
		rt_sem_signal(&mux2);
		
		rt_task_wait_period();
	}
}



void tsk2_job(long arg){
	while(1){
		//rt_printk("Task 4 resumed");
		send_log(2, 20);
		
		
		rt_change_prio(rt_whoami(), 1);
		send_log(2, 21);
		
		rt_sem_wait(&mux1);
		send_log(2, 22);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		rt_change_prio(rt_whoami(), 2);
		send_log(2, 23);
		
		rt_sem_signal(&mux1);
		
		busy_sleep(rand(5, 10) * MILLISEC);
		send_log(2, 24);
		
		rt_change_prio(rt_whoami(), 1);
		send_log(2, 25);
		
		rt_sem_wait(&mux2);
		send_log(2, 26);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(2, 27);
		
		rt_change_prio(rt_whoami(), 2);
		rt_sem_signal(&mux2);
		
		rt_task_wait_period();
	}
}


void tsk3_job(long arg){
	while(1){
		//rt_printk("Task 5 resumed");
		send_log(3, 30);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(3, 31);
		
		rt_change_prio(rt_whoami(), 1);
		send_log(3, 32);
		
		
		rt_sem_wait(&mux1);
		send_log(3, 33);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		rt_change_prio(rt_whoami(), 3);
		send_log(3, 34);
		
		rt_sem_signal(&mux1);
		
		rt_task_wait_period();
	}
}

int init_module(void){
	rt_printk("Started \n");
	
	int res; 
	res = rtf_create(FIFO_ID_LOG, sizeof(log)*(LOG_VAL));
	if(res == ENODEV){
		rt_printk("FIFO ID too large");
		return -1;
	}
	if (res == ENOMEM){
		rt_printk("FIFO size too large");
		return -2;
	}
	

	
	calibrate_busysleep(MAX_VAL, STEP, 3);
	
	long long start1, start2, start3, time;
	
	/* IPC initialization */
	rt_typed_sem_init(&mux1, 1, BIN_SEM);
	rt_typed_sem_init(&mux2, 1, BIN_SEM);


	
	/* Task initialization */
	rt_task_init(&tsk1, (void*)tsk1_job, 0, 10000, 1, 0, 0);
	rt_task_init(&tsk2, (void*)tsk2_job, 0, 10000, 2, 0, 0);
	rt_task_init(&tsk3, (void*)tsk3_job, 0, 10000, 3, 0, 0);


	rt_printk("Tasks initializated");
		
		
	rt_set_periodic_mode();
	start_rt_timer(nano2count(500000));
	time = rt_get_cpu_time_ns();

	start1 = nano2count(time + 1000*MILLISEC + 80 * MILLISEC);
	start2 = nano2count(time + 1000*MILLISEC + 100 * MILLISEC);
	start3 = nano2count(time + 1000*MILLISEC + 120 * MILLISEC);
	
	
	log_len = 0;
	
	/* Sending the start-time */
	s = time + 1000*MILLISEC;
	res = rtf_create(FIFO_ID_VALUES, sizeof(long long));
	if(res == ENODEV){
		rt_printk("FIFO ID too large");
		return -1;
	}
	if (res == ENOMEM){
		rt_printk("FIFO size too large");
		return -2;
	}
	rtf_put(FIFO_ID_VALUES, &s, sizeof(long long) );	
	
	/* Task execution */
	rt_task_make_periodic(&tsk1, start1, nano2count(80*MILLISEC)); 
	rt_task_make_periodic(&tsk2, start2, nano2count(100*MILLISEC));
	rt_task_make_periodic(&tsk3, start3, nano2count(120*MILLISEC));

	
	rt_printk("Tasks started");
	/* Comment next line for "standard" FIFO scheduling, uncomment for Rate Monotonic Scheduling*/
	//rt_spv_RMS(0);
		
	return 0;
}
void cleanup_module(void){

	/* stops threads */
	rt_task_delete(&tsk1);
	rt_task_delete(&tsk2);
	rt_task_delete(&tsk3);
	
	rt_sem_delete(&mux1);
	rt_sem_delete(&mux2);
	
	stop_rt_timer();
	rtf_destroy(FIFO_ID_LOG);
	rtf_destroy(FIFO_ID_VALUES);
	printk("Removed kernel module taskset.ko\n");
}
