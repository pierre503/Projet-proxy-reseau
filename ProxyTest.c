#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

#define PORTForServer 80
#define PORTForClient 6666
#define MAXClient 5

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;



void client(){
	int n = 0;
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		error("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORTForClient);
	if(bind(sockfd,(SOCKADDR *) &serv_addr,sizeof(serv_addr)) == SOCKET_ERROR)
   	{
      		perror("bind()");
      		exit(errno);
   	}
	listen(sockfd,MAXClient);
	while(1){
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
	
		bzero(buffer,1024);
		n = read(newsockfd,buffer,1024);
		
		if (n < 0) error("ERROR reading from socket");
	
		printf("Here is the message: %s\n",buffer);
		
		char s[2] = "\n";
		char host2[1024];
		char *host;
   		int i = 2;
		/* decoupage des element dont on a besoin*/
		host = strtok(buffer, s);
		host = strtok(NULL, s);
		printf( "avant \n");
		strcpy(host2,host);
		printf( "ici \n");
		printf( " %s\n", host );
    		char *hostname ;	
		hostname = strtok(host2, ": ");
		hostname = strtok(NULL, ": ");
		
		printf( " %s\n", hostname );

		
		printf( " yes :%s\n", hostname );



			
		SOCKET socketForServer = socket(AF_INET, SOCK_STREAM, 0);
	
		if(socketForServer == INVALID_SOCKET)
		{
			perror("socket()");
			exit(errno);
		}

		struct hostent *hostinfo = NULL;
		SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */
		hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */

		if (hostinfo == NULL) /* l'hôte n'existe pas */
		{
			fprintf (stderr, "Unknown host %s.\n", hostname);
			exit(EXIT_FAILURE);
		}

		sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
		sin.sin_port = htons(PORTForServer); /* on utilise htons pour le port */
		sin.sin_family = AF_INET;
		printf( "aie\n");
		if(connect(socketForServer,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			perror("connect()");
			exit(errno);
		}
		printf( "hello \n");
		
		printf( " %s\n", host );
		char buff[2024];
		sprintf(buff,"GET / HTTP/1.1\r\n%s\r\n\r\n",host);
		printf( " %s\n",buff);
		printf( "bye \n");
		if(write(socketForServer, buff, strlen(buff)) < 0)
		{
			perror("send()");
			exit(errno);
		}
		char bufferForSite[1024];


		if((n = read(socketForServer, bufferForSite, sizeof bufferForSite - 1)) < 0)
		{
			perror("recv()");
			exit(errno);
		}

		bufferForSite[n] = '\0';
		n = write(newsockfd,bufferForSite,sizeof(bufferForSite));

		if (n < 0) error("ERROR writing to socket");
	
		printf("%s\n",bufferForSite);
	
		closesocket(socketForServer);
		close(newsockfd);
	}
	close(sockfd);
}

int main(int argc, char *argv[]){
	client();

	return 0;
}



