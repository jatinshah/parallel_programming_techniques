#include "main.h"

void barrier_init(barrier_t *b, int nthrs){
	b->c = new pthread_cond_t;
	b->m = new pthread_mutex_t;
	pthread_cond_init(b->c, NULL);
	pthread_mutex_init(b->m, NULL);
	b->state=0;
	if(nthrs > 0) 
		b->nthreads=b->thr_rem[0]=b->thr_rem[1]=nthrs;
	else{
		errno=EINVAL;
		perror("barrier_init");
	}
		
}

void barrier(barrier_t *b){
	pthread_mutex_lock(b->m);
	b->thr_rem[b->state]--;
	if(b->thr_rem[b->state]==0){
		b->thr_rem[b->state]=b->nthreads;
		b->state=(b->state+1)%2;
		pthread_cond_broadcast(b->c);
		pthread_mutex_unlock(b->m);
	}
	else{
		pthread_cond_wait(b->c,b->m);
		pthread_mutex_unlock(b->m);
	}
}

void barrier_destroy(barrier_t *b){
	pthread_cond_destroy(b->c);
	pthread_mutex_destroy(b->m);
	delete b->c;
	delete b->m;
}
