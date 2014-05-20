#define SIZE 200
#include <stdio.h>
main(){
	double *A,*B,*C;
	int i,j;
	A=(double*)malloc(SIZE*SIZE*sizeof(double));
	B=(double*)malloc(SIZE*SIZE*sizeof(double));
	C=(double*)malloc(SIZE*SIZE*sizeof(double));

	for(i=0;i<SIZE;i++) for(j=0;j<SIZE;j++){
		C[i*SIZE+j]=0;
		A[i*SIZE+j]=B[i*SIZE+j]=i+j;
	}
	matmul(SIZE,A,B,C);
}
