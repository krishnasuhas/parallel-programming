#include<mpi.h>


#define YESMSG 1
#define NOMSG 0


int n,nnodes,me;
double t1,t2;


void init(int argc,char **argv)
{
	n=atoi(argv[1]);
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nnodes);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);	
	if(me==nnodes-1)t1=MPI_Wtime();
}
	

void node0()
{
	int tocheck,dummy,divisor=3;
	for(int i=1;i*2<=n;i++)
	{
		tocheck=i*2+1;
		if(tocheck>n)break;
		else
		{
			if(tocheck%3)
			MPI_Send(&tocheck,1,MPI_INT,me+1,YESMSG,MPI_COMM_WORLD);
		}
	}
	MPI_Send(&dummy,1,MPI_INT,me+1,NOMSG,MPI_COMM_WORLD);
}

void nodebtwn()
{
	int tocheck,dummy,divisor;
	MPI_Status status;
	MPI_Recv(&divisor,1,MPI_INT,me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);	
	
	do
	{
		MPI_Recv(&tocheck,1,MPI_INT,me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		if(status.MPI_TAG==NOMSG)
		break;
		else if(tocheck%divisor)
		MPI_Send(&tocheck,1,MPI_INT,me+1,YESMSG,MPI_COMM_WORLD);
	}while(1);
	MPI_Send(&dummy,1,MPI_INT,me+1,NOMSG,MPI_COMM_WORLD);
}

void nodeend()
{
	int tocheck,dummy,divisor,nprimes=3,flag;
	MPI_Status status;
	MPI_Recv(&divisor,1,MPI_INT,me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	printf("num of nodes : %d\n",nnodes);	
	printf("2\t3\t%d\t",divisor);
	do
	{
		MPI_Recv(&tocheck,1,MPI_INT,me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		if(status.MPI_TAG==NOMSG)
		break;
		else
		{
			flag=0;
			
			for(int i=divisor;i<tocheck;i+=2)
			{
				if(tocheck%i==0)
				{
					flag=1;
					break;
				}				
			}
			if(flag==0)
			{
				printf("%d\t",tocheck);
				 nprimes++;
			}
		}
	}while(1);
	printf("%d primes found\n",nprimes);
	t2=MPI_Wtime();
	printf("elapsed time =%f\n",(float)(t2-t1));
}

int main(int argc,char **argv)
{
	init(argc,argv);

	for(int i=0;i<=nnodes;i++)
	{
		if(me==i)
		node0();
		else if(me==nnodes-1)
		nodeend();
		else
		nodebtwn();
	}
	MPI_Finalize();
}












