#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,9);
    
    
    int nombre1,nombre2;
    char operateur;
    scanf("%d %d %c", &nombre1, &nombre2, &operateur);
    //on écrit l'entier sur 4 bytes
    buffer[0]= nombre1 & 0xff; //8 bits de poids faibles 
    buffer[1]= (nombre1 >> 8) & 0xff;
    buffer[2]= (nombre1 >> 16) & 0xff;
    buffer[3]= (nombre1 >> 24) & 0xff;//8 bits de poids fort  
    buffer[4]= nombre2 & 0xff; //8 bits de poids faibles 
    buffer[5]= (nombre2 >> 8) & 0xff;
    buffer[6]= (nombre2 >> 16) & 0xff;
    buffer[7]= (nombre2 >> 24) & 0xff;//8 bits de poids fort   
	buffer[8]= operateur & 0xff;
    
    //fgets(buffer,255,stdin);


    n = write(sockfd,buffer,9);
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,8);
    n = read(sockfd,buffer,256);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
