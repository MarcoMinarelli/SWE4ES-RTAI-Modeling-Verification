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



long rand(int min, int max){
	unsigned random_val;
	get_random_bytes(&random_val, sizeof(unsigned));
	random_val = min*100 + (random_val % max*100);
	return (long) random_val/100;
}

/* Variables for task creation */ 
RT_TASK tsk1, tsk2, tsk3, tsk4, tsk5;

SEM mux1, mux2;
MBX mbx;

void send_log(int task_id, int trans_id){
	log l;
	l.task_id = task_id;
	l.transition_id = trans_id;
	l.time = rt_get_cpu_time_ns();
	rtf_put(FIFO_ID_LOG, &l, sizeof(l) );	
}

void tsk1_job(long arg){
	message m;

	while(1){
		send_log(1, 10);
	
		if(rt_mbx_receive(&mbx, &m, sizeof(message)) == 0){
			if(m.dest == 1 && m.send == 2){
				send_log(1, 11);
				busy_sleep(rand(5, 10) * MILLISEC);
				send_log(1, 12);
			}
		}else{
			rt_printk("Message not received");
		}
		rt_task_wait_period();
	}
}


void tsk2_job(long arg){
	message m;
	m.send = 2;
	m.dest = 1;
	int prio = rt_get_prio(rt_whoami());
	while(1){
		send_log(2, 20);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(2, 21);
		

		if(!rt_mbx_send_if(&mbx, &m, sizeof(message)) == 0){
			rt_printk("Message not sended");
		}

		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(2, 22);
		
		rt_task_wait_period();
	}
}


void tsk3_job(long arg){
	while(1){
		send_log(3, 30);
		
		rt_sem_wait(&mux1);
		send_log(3, 31);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(3, 32);
		
		rt_sem_signal(&mux1);
		
		busy_sleep(rand(5, 10) * MILLISEC);
		send_log(3, 33);
		
		rt_sem_wait(&mux2);
		send_log(3, 34);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(3, 35);
		
		rt_sem_signal(&mux2);
		
		rt_task_wait_period();
	}
}



void tsk4_job(long arg){
	while(1){
		send_log(4, 40);
		
		
		rt_change_prio(rt_whoami(), 3);
		send_log(4, 41);
		
		rt_sem_wait(&mux1);
		send_log(4, 42);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		rt_change_prio(rt_whoami(), 4);
		send_log(4, 43);
		
		rt_sem_signal(&mux1);
		
		busy_sleep(rand(5, 10) * MILLISEC);
		send_log(4, 44);
		
		rt_change_prio(rt_whoami(), 3);
		send_log(4, 45);
		
		rt_sem_wait(&mux2);
		send_log(4, 46);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(4, 47);
		
		rt_change_prio(rt_whoami(), 4);
		rt_sem_signal(&mux2);
		
		rt_task_wait_period();
	}
}


void tsk5_job(long arg){
	while(1){
		
		send_log(5, 50);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		send_log(5, 51);
		
		rt_change_prio(rt_whoami(), 3);
		send_log(5, 52);
		
		
		rt_sem_wait(&mux1);
		send_log(5, 53);
		
		busy_sleep(rand(1, 2) * MILLISEC);
		rt_change_prio(rt_whoami(), 5);
		send_log(5, 54);
		
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
	
	long long start1, start2, start3, start4, start5, time;
	
	/* IPC initialization */
	rt_typed_sem_init(&mux1, 1, BIN_SEM);
	rt_typed_sem_init(&mux2, 1, BIN_SEM);

	rt_typed_mbx_init(&mbx, sizeof(message), PRIO_Q);
	
	/* Task initialization */
	rt_task_init(&tsk1, (void*)tsk1_job, 0, 10000, 1, 0, 0);
	rt_task_init(&tsk2, (void*)tsk2_job, 0, 10000, 2, 0, 0);
	rt_task_init(&tsk3, (void*)tsk3_job, 0, 10000, 3, 0, 0);
	rt_task_init(&tsk4, (void*)tsk4_job, 0, 10000, 4, 0, 0);
	rt_task_init(&tsk5, (void*)tsk5_job, 0, 10000, 5, 0, 0);


		
		
	rt_set_periodic_mode();
	start_rt_timer(nano2count(500000));
	time = rt_get_cpu_time_ns();

	start1 = nano2count(time + 1000*MILLISEC + 40 * MILLISEC);
	start2 = nano2count(time + 1000*MILLISEC + 40 * MILLISEC);
	start3 = nano2count(time + 1000*MILLISEC + 80 * MILLISEC);
	start4 = nano2count(time + 1000*MILLISEC + 100 * MILLISEC);
	start5 = nano2count(time + 1000*MILLISEC + 120 * MILLISEC);
	
	
	
	
	/* Sending the start-time */
	long long s = time + 1000*MILLISEC;
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
	rt_task_make_periodic(&tsk1, start1, nano2count(40*MILLISEC)); 
	rt_task_make_periodic(&tsk2, start2, nano2count(40*MILLISEC));
	rt_task_make_periodic(&tsk3, start3, nano2count(80*MILLISEC));
	rt_task_make_periodic(&tsk4, start4, nano2count(100*MILLISEC));
	rt_task_make_periodic(&tsk5, start5, nano2count(120*MILLISEC));
	
	
	/* Comment next line for "standard" FIFO scheduling, uncomment for Rate Monotonic Scheduling*/
	//rt_spv_RMS(0);
		
	return 0;
}
void cleanup_module(void){

	/* stops threads */
	rt_task_delete(&tsk1);
	rt_task_delete(&tsk2);
	rt_task_delete(&tsk3);
	rt_task_delete(&tsk4);
	rt_task_delete(&tsk5);
	
	rt_mbx_delete(&mbx);
	rt_sem_delete(&mux1);
	rt_sem_delete(&mux2);
	
	stop_rt_timer();
	rtf_destroy(FIFO_ID_LOG);
	rtf_destroy(FIFO_ID_VALUES);
	printk("Removed kernel module taskset.ko\n");
}
