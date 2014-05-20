#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_PROFIT_LIMIT 100
#define max(a,b) (a>b?a:b)

typedef enum {INCLUDED,NOT_INCLUDED,UNSET} selector;
typedef struct{
	selector s;
	int cost;
}table_entry;
typedef struct{
	int rank;
	int max_balls;
	int knapsack_capacity;
}process_data;

//Global Variables
table_entry *table;
int *profit;
int *weight;
pthread_mutex_t *lock;		//mutex for each column of Table
pthread_cond_t *cond;		//condition variables

void *start_thread(void *arg)
{
	process_data *data=(process_data*)arg;
	int rank=data->rank;
	int max_balls=data->max_balls;
	int knapsack_capacity=data->knapsack_capacity;
	for(int i=0;i<data->max_balls;i++){
		int cost_with_i,cost_without_i;
		if(rank-weight[i] < 0)
			cost_with_i=0;
		else if(i == 0 && rank-weight[i] >= 0)
			cost_with_i=profit[i];
		else{
			pthread_mutex_lock(&lock[rank-weight[i]]);
			if(table[(i-1)*knapsack_capacity+rank-weight[i]].s == UNSET)
				pthread_cond_wait(&cond[rank-weight[i]],&lock[rank-weight[i]]);
			cost_with_i=table[(i-1)*knapsack_capacity+rank-weight[i]].cost+profit[i];
			pthread_mutex_unlock(&lock[rank-weight[i]]);
		}
		cost_without_i=(i==0)?0:table[(i-1)*knapsack_capacity+rank].cost;

		table[i*knapsack_capacity+rank].cost=max(cost_with_i,cost_without_i);
		table[i*knapsack_capacity+rank].s=(table[i*knapsack_capacity+rank].cost==cost_without_i)?NOT_INCLUDED:INCLUDED;
		pthread_cond_broadcast(&cond[rank]);
	}
	free(data);
}
int main(int argc,char *argv[])
{
	if(argc != 4){
		printf("Error: knapsack <max capacity> <max balls> <seed>\n");
		exit(0);
	}
	printf("Initializing...\n");
	int knapsack_capacity=atoi(argv[1])+1;
	int max_balls=atoi(argv[2]);
	table=(table_entry*)malloc(knapsack_capacity*max_balls*sizeof(table_entry));
	profit=(int*)malloc(max_balls*sizeof(int));
	weight=(int*)malloc(max_balls*sizeof(int));
	lock=(pthread_mutex_t*)malloc(knapsack_capacity*sizeof(pthread_mutex_t));
	cond=(pthread_cond_t*)malloc(knapsack_capacity*sizeof(pthread_cond_t));
	//Assign Costs randomly to balls
	srandom(atoi(argv[3]));
	for(int i=0;i<max_balls;i++){
		profit[i]=random()%MAX_PROFIT_LIMIT;
		weight[i]=random()%(knapsack_capacity-1)+1;
		printf("Profit[%d]:%d Weight[%d]:%d\n",i,profit[i],i,weight[i]);
	}

	//Set all entries of table as unset
	for(int i=0;i<max_balls;i++) for(int j=0;j<knapsack_capacity;j++)
		table[i*knapsack_capacity+j].s=UNSET;

	//Initialize the lock
	for(int i=0;i<knapsack_capacity;i++)
		pthread_mutex_init(&lock[i],NULL);
	
	//Initialize the lock
	for(int i=0;i<knapsack_capacity;i++)
		pthread_cond_init(&cond[i],NULL);

	//Create threads and allocate jobs
	pthread_t *thr = (pthread_t*)malloc(knapsack_capacity*sizeof(pthread_t));
	printf("Creating Threads...\n");
	for(int i=0;i<knapsack_capacity;i++){
		process_data *data=(process_data*)malloc(sizeof(process_data));
		data->knapsack_capacity=knapsack_capacity;
		data->max_balls=max_balls;
		data->rank=i;
		pthread_create(&thr[i],NULL,start_thread,(void*)data);
	}

	for(int i=0;i<knapsack_capacity;i++)
		pthread_join(thr[i],NULL);

	//Print results
	for(int i=0;i<max_balls;i++)
		for(int j=0;j<knapsack_capacity;j++)
			printf("Table[%d][%d]:%d\n",i,j,table[i*knapsack_capacity+j].cost);
	//Print the balls selected
	printf("Knapsack:%d Balls:%d\nBalls: ",knapsack_capacity-1,max_balls);
	int c=knapsack_capacity-1;
	for(int i=max_balls-1;c>0&&i>=0;i--){
		if(table[i*knapsack_capacity+c].s == INCLUDED){
			printf("%d ",i);
			c-=weight[i];
		}
	}
	printf("\n");
	//Finalizer
	printf("Finalizing...\n");
	for(int i=0;i<knapsack_capacity;i++)
		pthread_mutex_destroy(&lock[i]);
	for(int i=0;i<knapsack_capacity;i++)
		pthread_cond_destroy(&cond[i]);
	free(table);
	free(profit);
	free(thr);
	free(lock);
	free(cond);
}
