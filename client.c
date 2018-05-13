#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char** argv)
{
	int server_socket;
	int portno;
	
	int i;
	int index=0;
	
	int *X;
	int n;
	float r;
	float average;
	int max_and_min[2];
	float *prod;
	
	struct sockaddr_in serv_addr;
	
	struct hostent *server;
	
	char buffer[256];
	
	portno=atoi(argv[2]);
	
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	
	server=gethostbyname(argv[1]);
	
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port=htons(portno);
	
	connect(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	
	////////////////////////////////////////////////////////
	//sending the number of elements (n)...
	////////////////////////////////////////////////////////
	printf("Give n to the server: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n=atoi(buffer);
	
	write(server_socket,buffer,strlen(buffer));
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////
	//sending the number r...
	////////////////////////////////////////////////////////
	printf("Give r to the server: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	r=atoi(buffer);
	
	write(server_socket,buffer,strlen(buffer));
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////
	//initializing and sending the array...
	////////////////////////////////////////////////////////
	X=(int *)malloc(n*sizeof(int));

	for(i=0;i<n;i++)
	{
		printf("X[%d] = ", i);
		scanf("%d", &X[i]);
	}
	
	bzero(buffer,256);
	for(i=0;i<n;i++)
		index+=sprintf(&buffer[index], "%d#", X[i]);

	write(server_socket,buffer,strlen(buffer));
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	printf("--------------------------------------------------------------------\n");
	
	
	////////////////////////////////////////////////////////
	//receiving average...
	////////////////////////////////////////////////////////
	bzero(buffer,256);
	read(server_socket,buffer,255);
	average=atof(buffer);
	printf("Average: %.2f \n", average);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////
	//receiving max and min elements...
	////////////////////////////////////////////////////////
	bzero(buffer,256);
	read(server_socket,buffer,255);
	
	i=0;
	
	char *p=strtok(buffer,"#");
	while(p!=NULL)
	{
		max_and_min[i++]=atoi(p);
		p=strtok(NULL,"#");	
	}

	printf("Max element: %d \nMin element: %d \n", max_and_min[0], max_and_min[1]);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////
	//receiving prod=r*X...
	////////////////////////////////////////////////////////
	prod=(float *)malloc(n*sizeof(float));
	
	bzero(buffer,256);
	read(server_socket,buffer,255);
	
	i=0;
	
	p=strtok(buffer,"#");
	while(p!=NULL)
	{
		prod[i++]=atof(p);
		p=strtok(NULL,"#");	
	}
	
	for(i=0;i<n;i++)
		printf("X[%d]*r = %.2f \n", i, prod[i]);
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	

	return 0;
}
