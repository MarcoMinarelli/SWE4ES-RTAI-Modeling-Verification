#include "rtai.h"


long double m, q;

void busy_sleep(RTIME time_ns){
	long n_cycles = (time_ns - q) / m;
	int dummy = 0;
	int i;
	for(i = 0; i < n_cycles; i++){
		dummy = dummy + 1;
	}
}

/* Function that given a certain number of iterations and a number of tests, compute the average over n_tests execution of a for loop of exactly 
	n_cycles iterations. */
RTIME calibrate_singlecall(long n_cycles, int n_tests){
	RTIME tests[n_tests] ;
	RTIME start_time;
	int k;
	for (k = 0; k < n_tests; k++){
		start_time = rt_get_cpu_time_ns();
		int dummy = 0;
		int i;
		for(i = 0; i < n_cycles; i++){
			dummy = dummy + 1;
		}
		tests[k] = rt_get_cpu_time_ns() - start_time;
	}
	
	/* compute average of the k tests */
	RTIME sum = 0;
	for (k = 0; k < n_tests; k++){
		sum += tests[k];
	}
	return sum/n_tests;
}

void calibrate_busysleep(long max_val, long step, int n_test){
	rt_printk("Started calibration process \n");
		
	long n = (int) max_val/step;

	long x;
	double y;
	
	double sum_xy = 0;
	double sum_x_sqr = 0;
	long sum_x = 0;
	double sum_y = 0;
	
	int index;
	long i; 
	for(i = 0, index = 1; i< max_val; i+=step, index++){
		/* get couples (n_cycles, avg_time) */
		x = i;
		y = (long double) calibrate_singlecall(i, n_test);
		
		/* Linear Regression */
	
		/* First we compute all needed quantities: sum of x_i * y_i, x and y, sum of x^2 */
		sum_xy += x * y;
		sum_x_sqr += x*x;
		sum_x += x;
		sum_y += y;
	}
	
	
	/* we can now compute m and q */
	m = (n * sum_xy - sum_x * sum_y )/ (n*sum_x_sqr - sum_x * sum_x );
	q = (sum_y - m * sum_x)/ n;
	
	rt_printk("Ended calibration.");
}
	
	
