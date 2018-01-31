#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int cmpints(int *a,int *b)
{
	if(*a>*b)
	return 1;
	else if(*b > *a)
	return -1;
	else
	return 0;
}


void add(int *a,int *count,int me,int value)
{
	int pos=count[me];
	a[pos]=	value;
	count[me]++;
}
	

void minmax(int *y,int *miny,int *maxy,int n)
{
	*miny=y[0];
	*maxy=y[0];
	
	for(int i=1;i<n;i++)
	{
		if(y[i]>*maxy)
		*maxy=y[i];
		else if(y[i]<*miny)
		*miny=y[i];
	}
}


void bsort(int *x,int n)
{
	float *boundries;
	int *nummypart;
	#pragma omp parallel
	{
		
		int *mypart=malloc(sizeof(int)*n);
		int me=omp_get_thread_num();
		int nthreads=omp_get_num_threads();	
		int minx,maxx;
		float 	increm;	
		boundries=malloc(sizeof(float)*nthreads-1);		
		nummypart=malloc(sizeof(int)*nthreads);		
		#pragma omp single
		{	
			minmax(x,&minx,&maxx,n);	
			increm=(maxx-minx)/(float)nthreads;			
			for(int i=0;i<nthreads-1;i++)
			{
				boundries[i]=minx+(i+1)*increm;
			}
		}
		nummypart[me]=0;		
		for(int i=0;i<n;i++)
		{

			if(me==0&&x[i]<boundries[me])
			{
					add(mypart,nummypart,me,x[i]);
			}
			else if(me<nthreads-1&&x[i]>=boundries[me-1]&&x[i]<boundries[me])
			{
				add(mypart,nummypart,me,x[i]);
			}
			else if(me==nthreads-1&&x[i]>=boundries[me-1])
			{
				add(mypart,nummypart,me,x[i]);
			}
		}
		#pragma omp barrier
		//qsort(mypart,nummypart[me],sizeof(int),cmpints);
		if(me==0)
		{
			for(int i=0;i<nummypart[me];i++)
			x[i]=mypart[i];
		}
		else if(me<nthreads-1)
		{
			int j=nummypart[me-1]-1;
			for(int i=j;i<j+nummypart[me];i++)
			x[i]=mypart[i-j];
		}
		else if(me==nthreads-1)
		{
			int j=nummypart[me-1]-1;
			for(int i=j;i<j+nummypart[me];i++)
			x[i]=mypart[i-j];
		}

	}
}
			
void main(int argc,char **argv)
{
	int n=atoi(argv[1]);
	int *x=malloc(sizeof(int) *n);
	
	for(int i=0;i<n;i++)
	{
		x[i]=rand()%50;
	}
	
	printf("before sort\n");
	
	for(int i=0;i<n;i++)
	{
		printf("%d\t",x[i]);
	}

	bsort(x,n);	
	
	printf("after sort\n");
	
	for(int i=0;i<n;i++)
	{
		printf("%d\t",x[i]);
	}
	
	free(x);
	return;
}
