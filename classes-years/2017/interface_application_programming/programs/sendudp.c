/*
* sendudp.c
* Neil Gershenfeld 2/5/11
*
* synatx: sendudp hostname string
*
* send string to hostname in a UDP packet on port 6543
*
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 6543

main(argc, argv)
   int argc;
   char *argv[]; {

   struct hostent *IPAddress;
   struct sockaddr_in Remote, Local;
   int Port = 6543, UDPtr, IPSize;
   char Address[100];
   
   IPAddress = gethostbyname(argv[1]);
   if (IPAddress == NULL) {
      printf("%s isn't in the name server\n",argv[1]);
      return; }
   strcpy(Address, inet_ntoa(*((struct in_addr *) *(IPAddress->h_addr_list))));
   
   bzero((char *) &Local, sizeof(Local));
   bzero((char *) &Remote, sizeof(Remote));
   Remote.sin_family = AF_INET;
   Remote.sin_addr.s_addr = inet_addr(Address);
   Remote.sin_port = htons(PORT);
   Local.sin_family = AF_INET;
   Local.sin_addr.s_addr = htonl(INADDR_ANY);
   Local.sin_port = htons(Port);
   UDPtr = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
   if (UDPtr < 0) {
      puts("Can't create the local socket");
      return; }
   if (bind(UDPtr, (struct sockaddr *) &Local, sizeof(Local)) < 0){
      puts("Can't bind local address");
      return; }
   IPSize = sendto(UDPtr, argv[2], strlen(argv[2]), 0, (struct sockaddr *) &Remote, sizeof(Remote));
   if (IPSize < 0) {
      puts("sendto error");
      return; }
   close(UDPtr);
   printf("Sent %d bytes to %s\n",IPSize, Address);
   }

