#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>         
#include <sys/wait.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 255
#define LISTEN_BACKLOG 50



int yes,nid,x,sfd;
char buf[SIZE];

void codeserveur(char *port, char *file){
   struct sockaddr_in myAddr, hisAddr;
   socklen_t addrlen;
   int fd,n;

   fd = open(file,O_WRONLY|O_CREAT|O_TRUNC,0666);

   if( fd == -1){
      perror("file open");
      exit(-1);
   } 

   bzero(buf,SIZE);
   bzero(&myAddr,sizeof(myAddr));

   sfd = socket(AF_INET, SOCK_STREAM,0);

   if(sfd == -1){
      perror("socket");
      exit(-1);
   }

   yes = 1;
   if(setsockopt(sfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1){
      perror("sockopt");
      exit(1);
   }

   myAddr.sin_family = AF_INET;
   myAddr.sin_addr.s_addr = INADDR_ANY;
   myAddr.sin_port = htons((short) strtol(port,NULL,0));

   addrlen = sizeof(struct sockaddr_in);

   if(bind(sfd, (struct sockaddr *)&myAddr, sizeof(myAddr)) != 0){
      perror("bind");
      exit(-1);
   }

   if(listen(sfd, LISTEN_BACKLOG) != 0){
      perror("listen");
      exit(-1);
   }

   nid = accept(sfd, (struct sockaddr*)&hisAddr, &addrlen);

   do{
      bzero(buf,SIZE);
      n = recv(nid,buf,SIZE,0);
      write(fd,buf,n);
   }while(n > 0);
}

void codeclient(char *ip, char *port, char *fileToSend){

   int fd = open(fileToSend, O_RDONLY);
   int n;

   if( fd == -1){
      perror("ouverture fichier");
      exit(-1);
   }

   int tranche = 100;

   struct sockaddr_in client;
   socklen_t peer_addr_size;

   sfd = socket(AF_INET, SOCK_STREAM, 0);

   if(sfd == -1){
      perror("socket");
      close(sfd);
      exit(1);
   }

   memset(&client,0, sizeof(struct sockaddr_in));

   peer_addr_size = sizeof(struct sockaddr_in);

   client.sin_family = AF_INET;
   client.sin_addr.s_addr = inet_addr(ip);//INADDR_ANY;
   client.sin_port = htons((short) strtol(port,NULL,0));


   yes = 1;
   if(setsockopt(sfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1){
      perror("sockopt");
      exit(1);
   }

   if(connect(sfd,(struct sockaddr*) &client,peer_addr_size) == -1){
      perror("connect");
      close(sfd);
      exit(1);
   }


   int i;

   do{
      bzero(buf,SIZE);
      n=read(fd,buf,tranche);

      if(send(sfd,buf,n,0) == -1){
         perror("sendto client");
         close(sfd);
         exit(1);
      }
   }while(n > 0);

}

int main(int argc, char *argv[]){

   if(argc < 2){
      printf("Usage: <%s> <-s or -c>\n",argv[0]);
      exit(1);
   }else{
      if(strcmp(argv[1],"-s") == 0){
         if(argc != 4){
            printf("Usage: <%s> <-s> <port> <filename to save>\n",argv[0]);
         }else{
            codeserveur(argv[2],argv[3]);
         }
      }

      if(strcmp(argv[1],"-c") == 0){
         if(argc < 5){
            printf("Usage: <%s> <-c> <hostname> <port> <filename to send>\n",argv[0]);
         }else{
            codeclient(argv[2],argv[3],argv[4]);
         }
      }
   }


   return EXIT_SUCCESS;
}
