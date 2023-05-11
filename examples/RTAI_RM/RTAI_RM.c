#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errno.h>
#include "rtai.h"
#include "rtai_sched.h"

MODULE_LICENSE("GPL"); 

#define MILLISEC 1000000

int retval;

/* Variables for task creation */ 
RT_TASK high_frequency_task;
RT_TASK mid_frequency_task;
RT_TASK low_frequency_task;

char names[3][3] = {"HF", "MF", "LF"};

void print_task(long taskID){
	while(1){
		rt_printk("I'm task %s \n", names[(int) taskID]);
		rt_task_wait_period();
	}
}

int init_module(void){
	
	printk(names[0]);
	/* Scheduling settings */
	rt_set_periodic_mode();
	start_rt_timer(nano2count(200000));
	long long time = rt_get_cpu_time_ns();
	
	
	/* Variables for period management */
	int period_hf_ms = 50, period_mf_ms = 200, period_lf_ms = 500;
	long long start_hf = nano2count(time + 1000*MILLISEC + period_hf_ms * MILLISEC);
	long long start_mf = nano2count(time + 1000*MILLISEC + period_mf_ms * MILLISEC);
	long long start_lf = nano2count(time + 1000*MILLISEC + period_lf_ms * MILLISEC);
	
	/* Period from ns to count */
	RTIME period_count_hf, period_count_mf, period_count_lf;
	period_count_hf = nano2count(period_hf_ms*MILLISEC);
	period_count_mf = nano2count(period_mf_ms*MILLISEC);
	period_count_lf = nano2count(period_lf_ms*MILLISEC);
	
	
	/*  Create real time threads */
	retval = rt_task_init(&high_frequency_task, print_task, 0, 1024, 1, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("HF: task structure is invalid\n"); 
		}else {
			printk("HF: error starting task\n");
		}
		return retval;
	}
	
	retval = rt_task_init(&mid_frequency_task, print_task, 1, 1024, 2, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("MF: task structure is invalid\n"); 
		}else {
			printk("MF: error starting task\n");
		}
		return retval;
	}
	retval = rt_task_init(&low_frequency_task, print_task, 2, 1024, 2, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("LF: task structure is invalid\n"); 
		}else {
			printk("LF: error starting task\n");
		}
		return retval;
	}
	printk("Initializated all tasks \n");
	
	
	/* Task execution */
	retval = rt_task_make_periodic(&high_frequency_task, start_hf, period_count_hf); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("HF: unvalid task structure\n");
		}else{ 
			printk("HF: error starting task\n"); 
		}		
		return retval;
	}

	retval = rt_task_make_periodic(&mid_frequency_task, start_mf, period_count_mf); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("MF: unvalid task structure\n");
		}else{ 
			printk("MF: error starting task\n"); 
		}
		return retval;
	}	
	
	retval = rt_task_make_periodic(&low_frequency_task, start_lf, period_count_lf); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("LF: unvalid task structure\n");
		}else{ 
			printk("LF: error starting task\n"); 
		}
		return retval;
	}	
	
	/* the next instruction tell the scheduler to set priorities inverse to the task period */
	rt_spv_RMS(0);
	
	printk("Started all tasks \n");
	
	return 0;
}
void cleanup_module(void){
	/* stops threads */
	retval = rt_task_delete(&high_frequency_task);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* invalid task structure */
			printk("HF : task structure is invalid\n");
		} else {
			printk("HF: error stopping task\n");
		}
	}
	
	retval = rt_task_delete(&mid_frequency_task);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* invalid task structure */
			printk("MF : task structure is invalid\n");
		} else {
			printk("MF: error stopping task\n");
		}
	}
	
	retval = rt_task_delete(&low_frequency_task);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* invalid task structure */
			printk("LF : task structure is invalid\n");
		} else {
			printk("LF: error stopping task\n");
		}
	}
	
	stop_rt_timer();
	printk("Removed kernel module RTAI_RM.ko\n");
}
