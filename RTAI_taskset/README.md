Objective: implement taskset represented in "Putting Preemptive Time Petri Nets to Work in a V-Model SW Life Cycle" (Carnevali, Vicario, Ridi 2011) and log the execution in a format that allows 
	to test feasibility.  
For the sake of simplicity, Task 5 is a periodic task with period 120 and not an asynchronous task.  

## Structure
In calib.h there are functions to calculate parameters of busy_sleep usin linear regression and the busy_sleep function, used to simulate a given execution time.  
In taskset.c there are function that implements tasks, some helper functions and modue entry and exit point.  
In values.h there are some values and structures shared between all other files, such as values for calibrate the busy_sleep function, and structure that represent a log, to be passed over a FIFO queue 
	from real time tasks to user space, low priority task ./read.  
In read.c the FIFO queue containing the log is read and it's values are written in files (see "Log structure" section).  

## How to compile
How to compile read.c : gcc -I . -fno-stack-protector read.c -o read  
How to compile the kernel module: make -f Makefile  

## How to execute
To start the module first load hal and sched module (sudo insmod /usr/realtime/modules/rtai_hal.ko && sudo insmod /usr/realtime/modules/rtai_sched.ko && sudo insmod /usr/realtime/modules/rtai_sem.ko sudo insmod /usr/realtime/modules/rtai_mbx.ko sudo insmod /usr/realtime/modules/rtai_fifos.ko), then type sudo insmod taskset.ko.  
In order to obtain the log, execute ./read  
When you're done, type sudo rmmod taskset

## Log structure
There will be 2 log files: log.txt (in which time of transition fires are relative to the time last transition fire) and log_abs.txt (in which times are relative to start time): 

in log.txt:  
	t10  
	40  
	t20  
	0  
	t21  
	2  
	t11  
	0  
	t12  
	8  

in log_abs.txt:  
	t10  
	40.002438  
	t20  
	40.005878  
	t21  
	41.872993  
	t11  
	41.874775  
	t12  
	49.878071  
	
