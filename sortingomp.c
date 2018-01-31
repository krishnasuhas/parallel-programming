#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

int cmpints(int *a,int *b)
{
	if(*a>*b)
	return 1;
	else if(*a<*b)
	return -1;	
	else
	return 0;
}


void grab(int *arr,int val,int *count)
{
	arr[*count]=val;
	*count=*count+1;
}


void minmax(int *arr,int *minx,int *maxx,int length)
{
	*minx=arr[0];
	*maxx=arr[0];
	int xi;
	for(int i=1;i<length;i++)
	{
		xi=arr[i];
		if(xi<*minx)	
		*minx=xi;
		if(xi<*maxx)	
		*maxx=xi;
	}
}


void bsort(int *n,int size)
{
	float *bdries;
	int *num;
	#pragma omp parallel
	{
		int *mypart;
		int me=omp_get_thread_num();
		int nthreads=omp_get_num_threads();
		int minx,maxx,increm;		
		#pragma omp single
		{
			
			bdries=malloc((nthreads-1)*sizeof(float));
			num=malloc((nthreads)*sizeof(int));
			minmax(n,&minx,&maxx,size);
			increm=(maxx-minx)/nthreads;
		}
		mypart=malloc(size*sizeof(int));
		bdries[me]=minx+(me+1)*increm;
		int nummypart=0;
		for(int i=0;i<size;i++)
		{
			if(me==0)
			{
				if(n[i]<bdries[me])
				grab(mypart,n[i],&nummypart);
			}
			else if(me<nthreads-1)
			{
				if(n[i]>bdries[me-1]&&n[i]<bdries[me])
				grab(mypart,n[i],&nummypart);
			}
			else
			{
				if(n[i]>bdries[nthreads-2])
				grab(mypart,n[i],&nummypart);
			}
		}
		qsort(n,size,sizeof(int),cmpints);
		num[me]=nummypart;		
		#pragma omp barrier
		int start;
		for(int i=0;i<me;i++)
		{
			start+=num[i];
		}
		for(int i=0;i<nummypart;i++)
		{
			n[start+i]=mypart[i];
		}
	}				
}


		

void main(int argc,char ** argv)
{
	int n=atoi(argv[1]);
	int *arr=malloc(n*sizeof(int));
	
	for(int i=0;i<n;i++)
	{
		arr[i]=rand()%100;
	}
	printf("before sort:\n");
	for(int i=0;i<n;i++)
	{
		printf("%d\t",arr[i]);
	}
	bsort(arr,n);
	printf("\nafter sort:\n");
	for(int i=0;i<n;i++)
	{
		printf("%d\t",arr[i]);
	}
	return;
}
