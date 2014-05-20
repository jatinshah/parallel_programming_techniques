#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#define NUMTHRS 10
#define MAXTIME 10
#define MAXBARRIERS 10


typedef struct{
	pthread_cond_t *c;
	pthread_mutex_t *m;
	int nthreads;
	int thr_rem[2]; // Threads remaining to invoke barrier
	int state;	// State which nthreads variable is used
} barrier_t;

typedef struct{
	int id;		// Process ID
	barrier_t *b;	
} proc_info_t;

void barrier_init(barrier_t *, int);
void barrier(barrier_t *);
void barrier_destroy(barrier_t *);
void *process_init(void *);
