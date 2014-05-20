// Consider Toroidal Grid in Sum function Calculation
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <mpi.h>

#define DEFAULT_SIZE 4
#define SLEEP_TIME 100L			// MicroSecs
#define RUN_TIME  10000L		// Running Time
#define GRID_DATA_FILE "grid.dat"
#define MAIN_PROC 0
#define UPDATE_TAG 0x11
#define NEWGRID_TAG 0x12

int filled_cells(int size,char *grid,int row,int col){
	int sum=0;
	for(int i=-1;i<2;i++)
		for(int j=-1;j<2;j++){
			if(row+i < 0 || row+i >= size || col+j < 0 || col+j >= size)
				continue;
			sum+=grid[(row+i)*size+(col+j)];
		}
	return sum-grid[row*size+col];
	
}

void print_grid(int size,char *grid){
	for(int i=0;i<size;i++){ 
		for(int j=0;j<size;j++)
			printf("%d ",grid[i*size+j]);
		printf("\n");
	}
	printf("\n");
}
	
int main(int argc,char *argv[])
{
	//Initialize the Grid
	MPI_Init(&argc,&argv);

	int numproc,rank,size;

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);

	if(argc >= 2)
		size=atoi(argv[1]);
	else
		size=DEFAULT_SIZE;

	char *grid=(char*)malloc(sizeof(char)*size*size);
	char *newgrid=(char*)malloc(sizeof(char)*size*size);

	FILE *fp=fopen(GRID_DATA_FILE,"r");
	for(int i=0;i<size;i++) for(int j=0;j<size;j++){
		fscanf(fp,"%d",&grid[i*size+j]);
		newgrid[i*size+j]=0;
	}
	fclose(fp);

	// Print Initial Grid
	if(rank == MAIN_PROC) print_grid(size,grid);
	int time=0;
	while(time<RUN_TIME){
		// Calculate New Grid -- Symmetric Work Distribution
		for(int i=rank;i<size*size;i+=numproc){
			int row=i/size;
			int col=i%size;
			int neighbours=filled_cells(size,grid,row,col);
			if(grid[row*size+col] == 0 && neighbours == 3)
				newgrid[row*size+col]=1;
			else{
				newgrid[row*size+col]=grid[row*size+col];
				if(neighbours <= 1)
					newgrid[row*size+col]=0;
				else if(neighbours >= 4)
					newgrid[row*size+col]=0;
			}
		}

		if(rank != MAIN_PROC){
			MPI_Status status;
			MPI_Request request;
			MPI_Isend(newgrid,size*size,MPI_CHAR,MAIN_PROC,NEWGRID_TAG,MPI_COMM_WORLD,&request);
			MPI_Wait(&request,&status);
		}
		// Process MAIN_PROC Calculates New Grid
		if(rank == MAIN_PROC){
			char *tmp=grid;
			grid=newgrid;
			newgrid=tmp;
			for(int i=0;i<numproc;i++){
				MPI_Status status;
				MPI_Request request;
				if(i == MAIN_PROC) continue;
				MPI_Irecv(newgrid,size*size,MPI_CHAR,i,NEWGRID_TAG,MPI_COMM_WORLD,&request);
				MPI_Wait(&request,&status);
				for(int j=0;j<size;j++) for(int k=0;k<size;k++)
					grid[j*size+k]|=newgrid[j*size+k];
			}
		// Process MAIN_PROC Prints the Grid
		print_grid(size,grid);
		}
		// Update Grid For Each Process
		MPI_Bcast(grid,size*size,MPI_CHAR,MAIN_PROC,MPI_COMM_WORLD);
		// Reset NewGrid
		for(int i=0;i<size;i++) for(int j=0;j<size;j++)
			newgrid[i*size+j]=0;
		time+=SLEEP_TIME;
		usleep(SLEEP_TIME);
	}
	MPI_Finalize();
}
