/*
* udprcv.c
* Neil Gershenfeld 2/5/11
*
* waits for an incoming UDP packet on port 6543 and prints it out
*
*/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define BufferSize 100
#define PORT 6543

main(void) {
   struct sockaddr_in Remote, Local;
   int UDPtr, IPSize, RemoteSize=sizeof(Remote);
   char Buffer[BufferSize];

   bzero((char *) &Buffer, sizeof(Buffer));
   UDPtr = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
   if (UDPtr < 0) {
      puts("Can't create the local socket");
      return; }
   Local.sin_family = AF_INET;
   Local.sin_addr.s_addr = htonl(INADDR_ANY);
   Local.sin_port = htons(PORT);
   if (bind(UDPtr, (struct sockaddr *) &Local, sizeof(Local)) < 0) {
      puts("UDPRecv: Can't bind local address");
      return; }
   IPSize = recvfrom(UDPtr, Buffer, BufferSize, 0,
      (struct sockaddr *) &Remote, &RemoteSize);
   if (IPSize < 0) {
      puts("UDPRecv: recvfrom error");
      return; }
   close(UDPtr);
   printf("Received %d bytes: %s\n",IPSize, Buffer);
   }
