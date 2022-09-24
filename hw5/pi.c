#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/syscall.h> 
#include <assert.h>
#include <string.h>
#include <math.h>
volatile double upper[64];
volatile double lower[64];


volatile int numCPU=1;
volatile int precision=7;
volatile double diff;


void thread(void *givenName) {
    int id = (intptr_t)givenName;
    double lowx=(double)id/(double)numCPU;
    double upx=(double)(id+1)/(double)numCPU;
    double len=upx-lowx;
    upper[id]=sqrt(1-upx*upx)*len;
    lower[id]=sqrt(1-lowx*lowx)*len;
    //printf("id=%d  upx=%lf  lowx = %lf\n",id,upx,lowx);
    //printf("id=%d  upper=%lf  lower = %lf\n",id,upper[id],lower[id]);
    for(int i=0;lower[id]-upper[id]>diff;i++)
    {
        //i = how many times cut
        //when cut upper /=2 and len/=2 and lower/=2 and add len*y for new area generated
        //for j=0~2^i upper+= len* y of(lowx+len*(2j+1))
        //for j=0~2^i upper+= len* y of(lowx+len*(2j))
        upper[id]/=2;
        lower[id]/=2;
        len/=2;
        long long int loops=pow(2,i);
        for(long long int j=0;j<loops;j++)
        {
            upper[id]+= len * sqrt(1-(lowx+len*(2*j+1))*(lowx+len*(2*j+1)));
            lower[id]+= len * sqrt(1-(lowx+len*(2*j))*(lowx+len*(2*j)));
        } 
        //if(id==0) printf("id=%d  upper=%.10lf  lower = %.10lf\n",i,upper[id],lower[id]);
    }
    



    
}

int main(int argc, char **argv) {

    if(argc>=2) precision=atoi(argv[1]);
    numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    if(argc>=3) numCPU=atoi(argv[2]);

    if(precision<5) precision=5;

    diff=pow(0.1,(double)precision);
    diff/=(numCPU);
    
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * numCPU);

    for (long i=0; i< numCPU; i++)
        pthread_create(&tid[i], NULL, (void *) thread, (void*)i);

    for (int i=0; i< numCPU; i++)
	    pthread_join(tid[i], NULL);


    double uppi=0,lowpi=0;
    for(int i=0;i<numCPU;i++)
    {
        uppi+=upper[i];
        //lowpi+=lower[i];
    }
    printf("precision=%d   pi=%.10lf\n",precision,uppi*4);

   
}
//total area of circle = half*half*pi assume there is a half=1 circle
//x*x+y*y=half y=sqrt(half-x-x)

