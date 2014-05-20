#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#define ABS(val) ((val) > 0 ? (val) : -(val))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define SQR(a)   ((a) * (a))
#define CUBE(a)  ((a) * (a) * (a))

//////////////////////////////////////////////////////////////////////
// BEGIN CONFIGURATION

#define NUM_CORRECTNESS_CHECKS 10
#define RANDOM_TESTS 0
#define MAX_ERROR 2.0

#define TEST_RUNS 30
#define CALC_ITERS(n) (10 + 1e8 / CUBE (n))

int qtest_sizes[] = { 16, 24, 32, 48, 64, 96, 128, 192, 256 }; // aligned sizes
int atest_sizes[] = { 23, 31, 47, 73, 97, 127, 163, 191, 211, 229, 251 };  // o dd sizes

/* primes from 16 to 256:
17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109
113 127 131 137 139 149 151 157 163 167 173 179 181 191 193 197 199 211
223 227 229 233 239 241 251 
*/

// END CONFIGURATION
//////////////////////////////////////////////////////////////////////

#define NUM_QTESTS (sizeof(qtest_sizes) / sizeof(int))
#define NUM_ATESTS (sizeof(atest_sizes) / sizeof(int))

int* test_sizes[2] = { qtest_sizes, atest_sizes };
int num_tests[2] = { NUM_QTESTS, NUM_ATESTS };

extern void matmul (int i_matdim,
                    const double* pd_A,
                    const double* pd_B,
                    double* pd_C);
#define MUL_MFMF_MF(size,A,B,C) matmul(size,A,B,C)

extern double drand48();
extern unsigned short* seed48();
extern int getrussage(int,struct rusage*);
struct rusage rus; /* starting time */
struct rusage rue; /* ending time */
#define START_TIMING  getrusage(RUSAGE_SELF,&rus);
#define STOP_TIMING   getrusage(RUSAGE_SELF,&rue);

double 
reportTiming() {
  struct timeval utime;
  utime.tv_sec = rue.ru_utime.tv_sec - rus.ru_utime.tv_sec ;
  if ( rue.ru_utime.tv_usec < rus.ru_utime.tv_usec ) {
    utime.tv_sec--;
    utime.tv_usec = 1000000l - rus.ru_utime.tv_usec +
      rue.ru_utime.tv_usec;
  } else
    utime.tv_usec = rue.ru_utime.tv_usec -
      rus.ru_utime.tv_usec ;
  return ((double)utime.tv_sec + (double)utime.tv_usec*1e-6);
}

void
myseed()
{
  int i;
  unsigned short seed16v[3];
  for (i=0;i<3;i++)
     seed16v[i] = time(0);
  seed48(seed16v);
}


/*
** A naive matrix multiply routine.
** Used to test for correctness.
*/
void
naive_mm(int Sm,int Sk,int Sn,
         const double *A,const double *B,double *C)
{
  int i,j,k;
  for (i=0;i<Sm;i++)
    for (j=0;j<Sn;j++) {
      double tmp = C[i*Sn+j];
      for (k=0;k<Sk;k++)
        tmp += A[i*Sk+k]*B[k*Sn+j];
      C[i*Sn+j] = tmp;
    }
}

/* return a random number uniformly in [l,u] inclusive, l < u */
int 
rrand(int l, int u) 
{ 
  return (l + (int)((1+u-l)*drand48())); 
}


double 
l1_norm(double *mat,int rows,int cols)
{
  double sum=0;
  int i;
  for (i=0;i<rows*cols;i++) {
    double val = *mat++;
    sum += ABS(val);
  }
  return sum;
}

double 
l1_norm_diff(double *mat1,double *mat2,int rows,int cols)
{
  double sum=0;
  int i;
  for (i=0;i<rows*cols;i++) {
    double val = *mat1++ - *mat2++;
    sum += ABS(val);
  }
  return sum;
}


/*
** error: Error formula to compare two 
**        matrix multiplies.
** norm(C1-C2)/(macheps*norm(A)*norm(B)),
** Ci=float(A*B)
** macheps=2^(-24) in single prec.
**        =2^(-53) in double prec.
*/
double 
error(double *mat1,double *mat2,int rows,int cols)
{
  const double macheps = 1.110223024625157e-16; /* = 2^(-53) */
  return l1_norm_diff(mat1,mat2,rows,cols)/
    (macheps*l1_norm(mat1,rows,cols)*l1_norm(mat2,rows,cols));
}


void 
mat_init(double *mat,int rows,int cols)
{
  int i;
  for (i=0;i<rows*cols;i++)
    *mat++ = 2.0*drand48()-1.0;
}

void 
checkCorrect ()
{
  double *A,*B,*C,*cA,*cB,*cC;
  int minDim = 1;
  int maxDim = 256;
  int maxnbytes = sizeof(double) * SQR (maxDim);
  int i, j;

  A = (double*) malloc (maxnbytes);
  B = (double*) malloc (maxnbytes);
  C = (double*) malloc (maxnbytes);
  
  cA = (double*) malloc (maxnbytes);
  cB = (double*) malloc (maxnbytes);
  cC = (double*) malloc (maxnbytes);
  
  fprintf (stderr, "Checking for correctness on sizes:"); 

#if !RANDOM_TESTS
  for (i = 0; i < 2; i++) for (j = 0; j < num_tests[i]; j++) 
    {
      int matdim = test_sizes[i][j];
#else
  for (i = 0; i < NUM_CORRECTNESS_CHECKS; i++) 
    {
      int matdim = rrand (minDim, maxDim);
#endif
      double err;
      int nbytes = sizeof(double) * SQR(matdim);

      fprintf (stderr, " %d", matdim); 

      mat_init (A, matdim, matdim);
      mat_init (B, matdim, matdim);
      mat_init (C, matdim, matdim);

      bcopy ((void*)A, (void*)cA, nbytes);
      bcopy ((void*)B, (void*)cB, nbytes);
      bcopy ((void*)C, (void*)cC, nbytes);

      naive_mm (matdim, matdim, matdim, cA, cB, cC);
      MUL_MFMF_MF (matdim, A, B, C);

      if (bcmp ((void*)A, (void*)cA, nbytes) != 0 ||
          bcmp ((void*)B, (void*)cB, nbytes) != 0) 
        {
          fprintf (stderr, "Source matrices were modified.  DISQUALIFIED!!!\n")
;
          //exit (0);
        }

      if ((err = error (C, cC, matdim, matdim)) > MAX_ERROR)
        {
          fprintf (stderr, "Error for test case %dx%d is %f > %f. DISQUALIFIED!
!!\n",
                   matdim, matdim, err, MAX_ERROR);
          //exit (0);
        }

    }
  fprintf (stderr,"\n"); 

  free (A); free (B); free (C); 
  free (cA); free (cB); free (cC);
}

void
timeIt ()
{
  double *A, *B, *C;
  double *oA[TEST_RUNS], *oB[TEST_RUNS], *oC[TEST_RUNS];
  int i, j, k;
  int test;

  for (k = 0; k < 2; k++)
    {
      if (k > 0) printf ("\n");

      for (test = 0; test < num_tests[k]; test++) 
        {
          int matdim = test_sizes[k][test];
          const int num_iters = CALC_ITERS (matdim);
          double max_mflops = 0.0;
          int run;
        
          /* make sure these are quad-word (i.e., 16-byte) aligned */
#if 0
          A = oA = (double*) malloc ((SQR(matdim)+1) * sizeof(double));
          B = oB = (double*) malloc ((SQR(matdim)+1) * sizeof(double));
          C = oC = (double*) malloc ((SQR(matdim)+1) * sizeof(double));
#endif
          
          for (run = 0; run < TEST_RUNS; run++) 
            {
              int iter;
              double mflops;
              double utime;

              /* use different matricies for each trial so that the OS page map
ping */
              /* won't affect the results... */
              A = oA[run] = (double*) malloc ((SQR(matdim)+rrand(1,10)) * sizeof(double));
              B = oB[run] = (double*) malloc ((SQR(matdim)+rrand(1,10)) * sizeof(double));
              C = oC[run] = (double*) malloc ((SQR(matdim)+rrand(1,10)) * sizeof(double));

              if (((unsigned)A) & 0x8) A = (double*)(((unsigned)A)+0x8);
              if (((unsigned)B) & 0x8) B = (double*)(((unsigned)B)+0x8);
              if (((unsigned)C) & 0x8) C = (double*)(((unsigned)C)+0x8);

              mat_init (A, matdim, matdim);
              mat_init (B, matdim, matdim);
              mat_init (C, matdim, matdim);

              START_TIMING;
              for (iter=0;iter<num_iters;iter++) 
                {
                  // iteratively accumulate into C
                  MUL_MFMF_MF (matdim, A, B, C);
                }
              STOP_TIMING;

              utime = reportTiming();
              // (2 * n^3) FLOPs (n^3 mul-adds)
              mflops = 2.0 * CUBE(matdim) * num_iters * 1e-6 / utime;
              max_mflops = MAX (max_mflops, mflops);
            }

          printf("%d %.0f\n", matdim, max_mflops); 
          fflush(stdout);

          for (run = 0; run < TEST_RUNS; run++) 
            {
              free (oA[run]); 
              free (oB[run]); 
              free (oC[run]); 
            }
        }
    }
}

int 
main()
{
  myseed();
  checkCorrect ();
  timeIt ();
  return 0;
}

