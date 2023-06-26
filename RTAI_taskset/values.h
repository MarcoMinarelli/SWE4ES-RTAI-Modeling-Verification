const int  FIFO_ID_VALUES = 0; //queue where the data will be stored
const int FIFO_ID_RESULT = 1;  //queue where the result (m, q)  will be stored
const int FIFO_ID_LOG = 2;     //queeu where the log info will be stored

const int  STEP = 10; //step for variable i between 2 tests 
const int  MAX_VAL = 200000;

const int  LOG_VAL = 20000;

#define MILLISEC 1000000

typedef struct{
	long x;
	double y;
}couple;

typedef struct{
	long double m;
	long double q;
}result;

typedef struct{
	int task_id;
	int transition_id;
	long long time;
}log;

typedef struct{
	int send;
	int dest;
}message;
