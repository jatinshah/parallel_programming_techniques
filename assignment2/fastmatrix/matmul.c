#include <stdlib.h>
#include <assert.h>
#define NUMSIZE 2

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
	double *B=pd_B;
	double *C=pd_C;

	for(i=0;i<Ntiles;i++)
		for(j=0;j<Ntiles;j++)
			for(k=0;k<Ntiles;k++){
				double *bA=A+i*n*N+k*n;
				double *bB=B+k*n*N+j*n;
				double *bC=C+i*n*N+j*n;
				bC[0]+=bA[0]*bB[0]+bA[1]*bB[N];
				bC[1]+=bA[0]*bB[1]+bA[1]*bB[N+1];
				bC[N]+=bA[N]*bB[0]+bA[N+1]*bB[N];
				bC[N+1]+=bA[N]*bB[1]+bA[N+1]*bB[N+1];
			}

	if((i_matdim&1) == 0)
		return;

	for(i=0;i<N-1;i++) for(j=0;j<N-1;j++){
		C[i*N+j]+=A[i*N+N-1]*B[(N-1)*N+j];
	}

	for(i=0;i<N;i++)
		for(k=0;k<N;k++)
			C[i*N+N-1]+=A[i*N+k]*B[k*N+N-1];

	for(i=0;i<N-1;i++)
		for(k=0;k<N;k++)
			C[(N-1)*N+i]+=A[(N-1)*N+k]*B[k*N+i];
}
