#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


main(int argc,char *argv[]){
	int i=random();
	printf("%d\n",i);
	MPI_Init(&argc,&argv);
	MPI_Finalize();
}
