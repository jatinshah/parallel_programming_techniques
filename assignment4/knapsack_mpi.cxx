#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_PROFIT_LIMIT 100
#define INCLUDED 0
#define NOT_INCLUDED 1
#define max(a,b) (a>b?a:b)
#define MAIN_PROC 0

typedef struct{
	int rank;
	int max_balls;
	int knapsack_capacity;
}process_data;


int main(int argc,char *argv[])
{
	MPI_Init(&argc,&argv);
	int numproc,rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);

	int knapsack_capacity,max_balls,seed;
	knapsack_capacity=atoi(argv[1])+1;
	max_balls=atoi(argv[2]);
	seed=atoi(argv[3]);
	
	if(argc != 4){
		printf("Error: knapsack <max capacity> <max balls> <seed>\n");
		exit(0);
	}else if(numproc != knapsack_capacity){
		printf("Error: Ensure capacity = processes-1\n");
		MPI_Finalize();
		exit(0);
	}

	//Begin Program
	int *profit;
	int *weight;
	//table_entry *table;		// Stores a column for each 
	int *table_cost;
	int *table_s;


	profit=(int*)malloc(sizeof(int)*max_balls);
	weight=(int*)malloc(sizeof(int)*max_balls);
	table_cost = (int*)malloc(sizeof(int)*max_balls);
	table_s = (int*)malloc(sizeof(int)*max_balls);

	srandom(seed);
	for(int i=0;i<max_balls;i++){
		profit[i]=random()%MAX_PROFIT_LIMIT;
		weight[i]=random()%(knapsack_capacity-1)+1;
		if(rank == MAIN_PROC)
			printf("Profit[%d] = %d Weight[%d]=%d\n",i,profit[i],i,weight[i]);
	}

	for(int i=0;i<max_balls;i++){
		int cost_with_i,cost_without_i;
		//Receive T[i-1][j-Wi]
		//Check i-1>=0 and j-Wi>=0 and rank>0
		if(rank-weight[i]<0)
			cost_with_i=0;
		else if(i == 0 && rank-weight[i] >= 0)
			cost_with_i=profit[i];
		else{
			MPI_Status status;
			MPI_Recv(&cost_with_i,1,MPI_INT,rank-weight[i],i-1,MPI_COMM_WORLD,&status);
			cost_with_i+=profit[i];
		}
		cost_without_i=(i==0)?0:table_cost[i-1];
		//Compute T[i][j]
		table_cost[i]=max(cost_with_i,cost_without_i);
		table_s[i]=(table_cost[i]==cost_without_i)?NOT_INCLUDED:INCLUDED;
		//Send to all procs with rank this_rank+Wi (in range), non-blocking
		MPI_Request request;
		for(int j=rank+1;j<numproc;j++){
			if(j-rank==weight[i+1]){
				MPI_Isend(&table_cost[i],1,MPI_INT,j,i,MPI_COMM_WORLD,&request);
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	// Assimilate Results and Print them
#define EVAL_TAG 0x20
	if(rank != knapsack_capacity-1){
		MPI_Request request;
		MPI_Isend(table_s,max_balls,MPI_INT,knapsack_capacity-1,EVAL_TAG,MPI_COMM_WORLD,&request);
	}
	else{
		printf("Knapsack Size:%d Balls:%d\n",knapsack_capacity-1,max_balls);
		printf("Profit:%d\n",table_cost[max_balls-1]);
		printf("Balls: ");
		int c=knapsack_capacity-1;
	
		for(int i=max_balls-1;c>0&&i>=0;i--){
			if(table_s[i] == INCLUDED){
				printf("%d ",i);
				c-=weight[i];
				MPI_Status status;
				if(c > 0)
					MPI_Recv(table_s,max_balls,MPI_INT,c,EVAL_TAG,MPI_COMM_WORLD,&status);
			}
		}
		printf("\n");
		
	}
	MPI_Finalize();
}
