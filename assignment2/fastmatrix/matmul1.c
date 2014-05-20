#include <stdlib.h>
#include <assert.h>
#define NUMSIZE 2

void transpose(int i_matdim,const double *pd_B,double *B)
{
	int i,j;
	for(i=0;i<i_matdim;i++)
		for(j=0;j<i_matdim;j++){
			B[i*i_matdim+j]=pd_B[j*i_matdim+i];
		}
}
void
matmul(int i_matdim,
	     const double* pd_A,
	     const double* pd_B,
	     double* pd_C)
{
	int i,j,k;
	int n=NUMSIZE;
	int N=i_matdim;
	int Ntiles=N/n;
	double *A=pd_A;
	double *B=(double*)malloc(i_matdim*i_matdim*sizeof(double));
	double *C=pd_C;

	transpose(i_matdim,pd_B,B);
	for(i=0;i<Ntiles;i++)
		for(j=0;j<Ntiles;j++)
			for(k=0;k<Ntiles;k++){
				double *bA=A+i*n*N+k*n;
				double *bB=B+j*n*N+k*n;
				double *bC=C+i*n*N+j*n;
				bC[0]+=bA[0]*bB[0]+bA[1]*bB[1];
				bC[1]+=bA[0]*bB[N]+bA[1]*bB[N+1];
				bC[N]+=bA[N]*bB[0]+bA[N+1]*bB[1];
				bC[N+1]+=bA[N]*bB[N]+bA[N+1]*bB[N+1];
			}

	if((i_matdim&1) == 0)
		return;

	for(i=0;i<N-1;i++) for(j=0;j<N-1;j++){
		C[i*N+j]+=A[i*N+N-1]*pd_B[(N-1)*N+j];
	}

	for(i=0;i<N;i++)
		for(k=0;k<N;k++)
			C[i*N+N-1]+=A[i*N+k]*pd_B[k*N+N-1];

	for(i=0;i<N-1;i++)
		for(k=0;k<N;k++)
			C[(N-1)*N+i]+=A[(N-1)*N+k]*pd_B[k*N+i];
}
