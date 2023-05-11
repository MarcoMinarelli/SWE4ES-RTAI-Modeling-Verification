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
RT_TASK print_task_1;
RT_TASK print_task_2;

void print_function1(long arg){
	while(1){
		rt_printk("Hello world! I'm task %d \n", arg);
		rt_task_wait_period();
	}
}


int init_module(void){
	
	
	/* Scheduling settings */
	rt_set_periodic_mode();
	start_rt_timer(nano2count(200000));
	long long time = rt_get_cpu_time_ns();
	
	
	/* Variables for period management */
	int period_t1_ms = 100, period_t2_ms = 300;
	long long start1 = nano2count(time + 1000*MILLISEC + period_t1_ms * MILLISEC);
	long long start2 = nano2count(time + 1000*MILLISEC + period_t2_ms * MILLISEC);
	
	/* Period from ns to count */
	RTIME period_count_t1, period_count_t2;
	period_count_t1 = nano2count(period_t1_ms*MILLISEC);
	period_count_t2 = nano2count(period_t2_ms*MILLISEC);
	
	
	/*  Create real time threads */
	retval = rt_task_init(&print_task_1, print_function1, 1, 1024, 1, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("task1: task structure is invalid\n"); 
		}else {
			printk("task1: error starting task\n");
		}
		return retval;
	}
	
	retval = rt_task_init(&print_task_2, print_function1, 2, 1024, 2, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("task2: task structure is invalid\n"); 
		}else {
			printk("task2: error starting task\n");
		}
		return retval;
	}
	printk("Initializated both tasks \n");
	
	
	/* Task execution */
	retval = rt_task_make_periodic(&print_task_1, start1, period_count_t1); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("task1: unvalid task structure\n");
		}else{ 
			printk("task1: error starting task\n"); 
		}		
		return retval;
	}

	retval = rt_task_make_periodic(&print_task_2, start2, period_count_t2); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("task2: unvalid task structure\n");
		}else{ 
			printk("task2: error starting task\n"); 
		}
		return retval;
	}	
	
	printk("Started both tasks \n");
	
	return 0;
}
void cleanup_module(void){
	/* stops threads */
	retval = rt_task_delete(&print_task_1);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* invalid task structure */
			printk("task1 : task structure is invalid\n");
		} else {
			printk("task1: error stopping task\n");
		}
	}
	
	retval = rt_task_delete(&print_task_2);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* invalid task structure */
			printk("task2 : task structure is invalid\n");
		} else {
			printk("task2: error stopping task\n");
		}
	}
	
	stop_rt_timer();
	printk("Removed kernel RTAI_periodic.ko\n");
}
