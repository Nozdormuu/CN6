#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")
#define PORT 27015
#define BUFFER_SIZE 1024

int main() {
   int sock;
   struct sockaddr_in serverAddr, clientAddr;
   int clientAddrLen = sizeof(clientAddr);
   char buffer[BUFFER_SIZE];

   
   WSADATA wsaData;
   WSAStartup(MAKEWORD(2, 2), &wsaData);

   
   sock = socket(AF_INET, SOCK_DGRAM, 0);

   
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_addr.s_addr = INADDR_ANY;
   serverAddr.sin_port = htons(PORT);

   
   bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

   printf("UDP Server is running on port %d...\n", PORT);

   while (true) {
      
      int bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
      buffer[bytesReceived] = '\0';
      printf("Received: %s\n", buffer);

         
         sendto(sock, buffer, bytesReceived, 0, (struct sockaddr*)&clientAddr, clientAddrLen);

      
      printf("Message sent back to client: %s\n", buffer);
   }

   closesocket(sock);
   WSACleanup();

   return 0;
}
