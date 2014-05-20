#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>


#define SAMPLE    10
#define CACHE_MIN (1024)
#define CACHE_MAX (1024*1024)

#define TIME_DIF_TO_NS(s,f) \
    ((f.tv_sec-s.tv_sec)*1000000000.0 + (f.tv_usec-s.tv_usec)*1000.0)

int x[CACHE_MAX];

void main()
{
    int register i, index, stride, limit, temp;
    double sample_ns, sample_sec, sec, ns;
    struct timeval start,finish;
    int steps, tsteps, csize;
  
    for (csize = CACHE_MIN; csize <= CACHE_MAX; csize *= 2){
        for (stride = 1; stride <= csize/2; stride *= 2){
	    sec = 0;
	    ns = 0.0;
	    limit = csize-stride+1;
	    steps = 0;
	    do {
		gettimeofday(&start, NULL);
		for (i = SAMPLE*stride; i != 0; i--)
		    for (index = 0; index < limit; index += stride)
			x[index]++;
		gettimeofday(&finish, NULL);
		sample_ns = TIME_DIF_TO_NS(start,finish);
		sample_sec = sample_ns/1000000000.0;
		steps++;
		sec += sample_sec;
		ns += sample_ns;
	    } while (sec < 1.0);
	    tsteps=0;
	    do {
		gettimeofday(&start, NULL);
		for (i = SAMPLE*stride; i != 0; i--)
		    for (index = 0; index < limit; index += stride)
			temp += index;
		gettimeofday(&finish, NULL);
		sample_ns = TIME_DIF_TO_NS(start,finish);
		sample_sec = sample_ns/1000000000.0;
		tsteps++;
		sec -= sample_sec;
		ns -= sample_ns;
	    } while (tsteps < steps);
	    printf("Size: %7d Stride: %7d read+write: %14.0f ns\n",
		   csize*sizeof(int),
		   stride*sizeof(int), 
		   (double) (sec*1000000000.0)/(steps*SAMPLE*stride*((limit-1.0)/stride+1.0)));
	}
	printf ("\n");
    }
    exit (0);
}

