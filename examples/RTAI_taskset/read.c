#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "values.h"

int main(){
	int fd;
	
	log l;
	int id;
	char path[] = "/dev/rtf";
	char log_id[5];
	char val_id[5];
	
	/* reading start time */
	sprintf(val_id, "%i", FIFO_ID_VALUES);
	strcat(path, val_id); 
	fd = open(path, O_RDONLY);
	long long start;
	read(fd, &start, sizeof(start));

	
	long long last_time = start;
	
	strcpy(path, "/dev/rtf");
	sprintf(log_id, "%i", FIFO_ID_LOG); 


	strcat(path, log_id); 
	
	/* Opens the txt file where we'll insert all avalues */
	FILE *f = fopen("log.txt", "w");
	if(f == NULL){
		printf("Error while opening");
	}
	
	FILE *f_abs = fopen("log_abs.txt", "w");

	fd = open(path, O_RDONLY);
	if(fd == -1){
		printf("Error opening FIFO values queue: %s", strerror(errno));
		return -1;
	}
	printf("Reading values... \n");
	/* Reading values from queue and printing on file */
	int i;
	for(i = 0; i < LOG_VAL; i++){
		if(read(fd, &l, sizeof(l)) > 0){
			id = l.task_id;
			
			if(fprintf(f, "t%d\n%lli\n", l.transition_id, (l.time - last_time)/MILLISEC) > 0){
				last_time = l.time;
			}else{
				printf("Error during writing opration on log.txt");
				return -1;
			}
			if(fprintf(f_abs, "t%d\n%f\n", l.transition_id, (float) (l.time - start)/MILLISEC) <= 0){
				printf("Error during writing opration on log_abs.txt");
				return -1;
			}
			
			if(i % 100 == 0){
				printf("Log %d read \n", i);
			}
		}
	}
	printf("Values printed on file \n");
	fclose(f);
	fclose(f_abs);
	
	/* Reading end time */
	strcpy(path, "/dev/rtf");
	sprintf(val_id, "%i", FIFO_ID_VALUES);
	strcat(path, val_id); 
	fd = open(path, O_RDONLY);
	long long end;
	read(fd, &end, sizeof(end));
	
	printf("Total execution time: %lli", (end - start)/MILLISEC);
	return 0;
}
