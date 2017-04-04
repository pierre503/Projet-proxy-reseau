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
	printf("hello\n");
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
	printf("hello2\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORTForClient);
	printf("hello?\n");
	if(bind(sockfd,(SOCKADDR *) &serv_addr,sizeof(serv_addr)) == SOCKET_ERROR)
   	{
      		perror("bind()");
      		exit(errno);
   	}
	printf("hello3\n");
	listen(sockfd,MAXClient);
	while(1){
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		printf("hello4\n");
		if (newsockfd < 0) 
			error("ERROR on accept");
	
		bzero(buffer,1024);
		n = read(newsockfd,buffer,1024);
		
		if (n < 0) error("ERROR reading from socket");
	
		printf("Here is the message: %s\n",buffer);	
		SOCKET socketForServer = socket(AF_INET, SOCK_STREAM, 0);
	
		if(socketForServer == INVALID_SOCKET)
		{
			perror("socket()");
			exit(errno);
		}

		struct hostent *hostinfo = NULL;
		SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */
		const char *hostname = "www.perdu.com";	
		hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */

		if (hostinfo == NULL) /* l'hôte n'existe pas */
		{
			fprintf (stderr, "Unknown host %s.\n", hostname);
			exit(EXIT_FAILURE);
		}

		sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
		sin.sin_port = htons(PORTForServer); /* on utilise htons pour le port */
		sin.sin_family = AF_INET;

		if(connect(socketForServer,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			perror("connect()");
			exit(errno);
		}

		char * buff = "GET / HTTP/1.1\r\nHost: www.perdu.com\r\n\r\n";
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



