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
	printf("Start time obtained \n");
	
	strcpy(path, "/dev/rtf");
	sprintf(log_id, "%i", FIFO_ID_LOG); 


	strcat(path, log_id); 
	
	/* Opens the txt file where we'll insert all avalues */
	FILE *f = fopen("log.txt", "w");
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
			
			fprintf(f, "t%d\n%lli\n", l.transition_id, (l.time - last_time)/MILLISEC);
			last_time = l.time;
			
			fprintf(f_abs, "t%d\n%f\n", l.transition_id, (float) (l.time - start)/MILLISEC);

			
			if(i % 1000 == 0){
				printf("Log %d read \n", i);
			}
		}
	}
	printf("Values printed on file \n");
	fclose(f);
	return 0;
}
