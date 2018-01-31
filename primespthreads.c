#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
				//it will primes in the given range by using threads.
int n,nthreads,nextbase;

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

pthread_t id[25];

int prime[100000000];

void crossout(int a)
{
	for(int i=3;i*a<=n;i++)
	prime[i*a]=0;
}

void * worker(int tn)
{
	int mywork=0,mybase;
	
	do
	{
		pthread_mutex_lock(&lock);
		mybase=nextbase;
		nextbase=nextbase+2;
		pthread_mutex_unlock(&lock);
		
		if(mybase<=n)
		{
			if(prime[mybase])
			{
				crossout(mybase);
				mywork++;
			}
		}
		else
		{
			
			return mywork;
		}
	}while(1);
}

void main(int argc,char**argv)
{
	n=atoi(argv[1]);
	nthreads=atoi(argv[2]);
	
	nextbase=3;

	for(int i=3;i<=n;i++)
	{
		if(i%2)prime[i]=1;
		else prime[i]=0;
	}

	for(int i=0;i<nthreads;i++)
	{
		pthread_create(&id[i],NULL,worker,i);
	}
	int work=0;
	for(int i=0;i<nthreads;i++)
	{
		pthread_join(id[i],&work);
		printf("work done by thread %d is %d\n",i,work);
	}
	int count=1;
	//printf("2\t");
	for(int i=3;i<=n;i++)
	{
		if(prime[i])
		{
			count++;
			//printf("%d\t",i);
		}
	}
	printf("num of primes are %d\n",count);
	return;
}
