#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

//mutual outbound links by using adjacency matrix

int **mat,n,count;

void teval(int **mat,int n)
{
	
	int total=0;
	int *mypart;
	int temp,nthreads;
	int *count;
	printf("%d",mat[0][1]);
	#pragma omp parallel
	{
		omp_set_num_threads(10);
		nthreads=omp_get_num_threads();	
		int me=omp_get_thread_num();		
		#pragma omp single
		{
			mypart=malloc(4*sizeof(int));
			temp=(n/(nthreads*2));
			count=malloc(nthreads*sizeof(int));
		}
		mypart[0]=temp*me;
		mypart[1]=temp*(me+1);
		mypart[3]=n-(temp*me);
		mypart[2]=n-(temp*(me+1));
		
		for(int i=mypart[0];i<mypart[1];i++)
		{
			for(int j=i+1;j<n;j++)
			{
				for(int k=0;k<n;k++)
				{
					if(mat[i][k]*mat[j][k])
					{
						count[me]++;
						//printf("pairs %d, %d, %d\n",i,j,k);
					}
				}
			}
		}
	
				
		for(int i=mypart[2];i<mypart[3];i++)
		{
			for(int j=i+1;j<n;j++)
			{
				for(int k=0;k<n;k++)
				{
					if(mat[i][k]*mat[j][k])
					{
						count[me]++;
						//printf("pairs %d, %d, %d\n",i,j,k);
					}
				}
			}
		}
		#pragma omp atomic
		total+=count[me];
	}
	
	printf("final result %d\n",total/(n*(n-1)/2));
}
		
			

void main(int argc,char **argv)
{
	n=atoi(argv[1]);
	mat=malloc(n*sizeof(int*));	
	for(int i=0;i<n;i++)
	{
		mat[i]=malloc(n*sizeof(int));
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			mat[i][j]=rand()%2;
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			printf("%d  ",mat[i][j]);
		}
		printf("\n");
	}
	teval(mat,n);
	return;
}

