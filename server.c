#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *session_function(void *arg)
{
	int client=(int) arg;
	
	int choice;
	
	int flag=1;
	
	int n;
	float r;
	int *X;
	float *prod;
	int i=0;
	int sum=0;
	float average;
	int max;
	int min;
	int max_and_min[2];

	printf("Connected.\n");

	do
	{
		recv(client,&choice,sizeof(int),0);
		printf("Choice from client: %d \n", choice);
		
		if(choice==1)
		{	
				////////////////////////////////////////////////////////
				//receiving the number of elements (n)...
				////////////////////////////////////////////////////////
				recv(client,&n,sizeof(int),0);
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
			
				
				////////////////////////////////////////////////////////
				//receiving the X array...
				////////////////////////////////////////////////////////
				X=(int *)malloc(n*sizeof(int));
			
				recv(client,X,n*sizeof(int),0);
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
		
		
				////////////////////////////////////////////////////////
				//calculating and sending average...
				////////////////////////////////////////////////////////
				printf("\tCalculating average of X[]...\n");
				
				for(i=0;i<n;i++)
					sum+=X[i];
				
				average=(float) sum/n;
					
				send(client,&average,sizeof(float),0);
				
				sum=0;
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
		}
		else if(choice==2)
		{

				////////////////////////////////////////////////////////
				//receiving the number of elements (n)...
				////////////////////////////////////////////////////////
				recv(client,&n,sizeof(int),0);
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
			
				
				////////////////////////////////////////////////////////
				//receiving the X array...
				////////////////////////////////////////////////////////
				X=(int *)malloc(n*sizeof(int));
			
				recv(client,X,n*sizeof(int),0);
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
				
				
				////////////////////////////////////////////////////////
				//finding and sending max and min elements...
				////////////////////////////////////////////////////////
				printf("\tFinding max and min of X[]...\n");
				
				max=X[0];
				min=X[0];
				
				for(i=0;i<n;i++)
				{
					if(X[i]>max)
						max=X[i];
				}
				
				for(i=0;i<n;i++)
				{
					if(X[i]<min)
						min=X[i];
				}
				
				max_and_min[0]=max;
				max_and_min[1]=min;

				send(client,max_and_min,2*sizeof(int),0);
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
		}
		else if(choice==3)
		{
				////////////////////////////////////////////////////////
				//receiving the number of elements (n)...
				////////////////////////////////////////////////////////
				recv(client,&n,sizeof(int),0);	
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
			
			
				////////////////////////////////////////////////////////
				//receiving the number r...
				////////////////////////////////////////////////////////
				recv(client,&r,sizeof(float),0);
				//////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
				
				
				////////////////////////////////////////////////////////
				//receiving the X array...
				////////////////////////////////////////////////////////
				X=(int *)malloc(n*sizeof(int));

				recv(client,X,n*sizeof(int),0);
				////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////


				////////////////////////////////////////////////////////
				//calculating and sending prod=r*X...
				////////////////////////////////////////////////////////
				printf("\tCalculating r*X[]...\n");

				prod=(float *)malloc(n*sizeof(float));

				for(i=0;i<n;i++)
					prod[i]=(float)X[i]*(float)r;
				
				send(client,prod,n*sizeof(float),0);

				for(i=0;i<n;i++)
					prod[i]=0;
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////	
		}
		else if(choice==4)
		{
				printf("Client exiting....\n");
				flag=0;
		}
		else
		{
				printf("Invalid Choice. Terminating in 3...2...1....\n");
		}
	}while(flag);
	
	close(client);
	pthread_exit(NULL);
}


int main(int argc, char** argv)
{
	int input_socket;
	int accepted_socket;
	int portno;
	int clilen;
	int i=0;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	
	pthread_t thread[50];

	input_socket = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno = atoi(argv[1]);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(portno);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	
	bind(input_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(input_socket,5);

	while(1)
	{
		printf("Waiting for connection...\n");
		clilen=sizeof(cli_addr);
		accepted_socket=accept(input_socket, (struct sockaddr *) &cli_addr, &clilen);
	
		pthread_create(&(thread[i++]), NULL, session_function, (void *) accepted_socket);
	}


	return 0;
}
