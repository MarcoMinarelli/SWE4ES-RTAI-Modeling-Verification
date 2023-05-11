#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errno.h> /* EINVAL, ENOMEM */
#include "rtai.h"
#include "rtai_sched.h"
#include <rtai_sem.h>

MODULE_LICENSE("GPL"); static RT_TASK print_task;


void print_function(long arg){
	rt_printk("Hello world!\n");
}


int init_module(void){
	int retval;
	rt_set_oneshot_mode(); start_rt_timer(1); //start execution
	
	/*  Create real time thread */
	retval = rt_task_init(&print_task, print_function, 0, 1024, RT_SCHED_LOWEST_PRIORITY, 0, 0);
	if ( retval != 0) {
		if (-EINVAL == retval) {
			printk("task: task structure is invalid\n"); 
		}else {
			printk("task: error starting task\n");
		}
		return retval;
	}
	retval = rt_task_resume(&print_task); 
	if (0 != retval) {
		if (-EINVAL == retval) {
			printk("struttura task invalida\n");
		}else{ 
		printk("task: error starting task\n"); }
		return retval;
	}
	return 0;
}
void cleanup_module(void){ }
