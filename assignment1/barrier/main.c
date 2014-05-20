#include "main.h"

void main(void){
	barrier_t *b = new barrier_t;
	barrier_init(b,NUMTHRS);
	
	pthread_t thr[NUMTHRS];
	for(int i=0;i<NUMTHRS;i++){
		proc_info_t *p = new proc_info_t;
		p->id = i;
		p->b = b;
		pthread_create(&thr[i],NULL,process_init,(void*)p);
	}
	void *status;
	for(int i=0;i<NUMTHRS;i++)
		pthread_join(thr[i],&status);
	barrier_destroy(b);
	delete b;
}

void *process_init(void *proc){
	proc_info_t *p = (proc_info_t *)proc;
	printf("Process %d Started\n",p->id);
//	 Random Time Break
	struct timespec time,rem;
	for(int i=0;i<MAXBARRIERS;i++){
		time.tv_sec=random()%MAXTIME;
		time.tv_nsec=0;
		nanosleep(&time,&rem);
		printf("Process %d invoking barrier %d\n",p->id,i);
		barrier(p->b);
		printf("Process %d restarted from barrier %d\n",p->id,i);
	}
	printf("Process %d exited\n",p->id);
	delete p;
}
