#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *session_function(void *arg)
{
	int client=(int) arg;
	
	int n;
	float r;
	int *X;
	float *prod;
	int i=0;
	int sum=0;
	float average;
	int max;
	int min;
	
	char data[100];
	int index=0;

	printf("Connected.\n");

	////////////////////////////////////////////////////////
	//receiving the number of elements (n)...
	////////////////////////////////////////////////////////
	recv(client,data,100,0);
	n=atoi(data);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	X=(int *)malloc(n*sizeof(int));

	////////////////////////////////////////////////////////
	//receiving the number r...
	////////////////////////////////////////////////////////
	recv(client,data,100,0);
	r=atof(data);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	prod=(float *)malloc(n*sizeof(float));
	
	////////////////////////////////////////////////////////
	//calculating and sending average...
	////////////////////////////////////////////////////////
	bzero(data,100);
	recv(client,data,100,0);
	
	char *p=strtok(data,"#");
	while(p!=NULL)
	{
		X[i++]=atoi(p);
		p=strtok(NULL,"#");
	}

	for(i=0;i<n;i++)
		sum+=X[i];

	average=(float) sum/n;
	bzero(data,100);
	sprintf(data,"%f", average);
	
	send(client,data,sizeof(data),0);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////
	//finding and sending max and min elements...
	////////////////////////////////////////////////////////
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
	
	bzero(data,100);
	sprintf(data,"%d#%d", max, min);

	sleep(1);
	send(client,data,sizeof(data),0);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////
	//calculating and sending prod=r*X...
	////////////////////////////////////////////////////////
	for(i=0;i<n;i++)
		prod[i]=(float)X[i]*(float)r;

	bzero(data,100);
	for(i=0;i<n;i++)
		index+=sprintf(&data[index], "%.2f#", prod[i]);
	
	sleep(1);
	send(client,data,sizeof(data),0);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
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
