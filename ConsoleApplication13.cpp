#undef UNICODE
#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 27015
#define BUFFER_SIZE 1024

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

int main() {
   int sock;
   struct sockaddr_in serverAddr;
   char buffer[BUFFER_SIZE];

   
   WSADATA wsaData;
   if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      printf("WSAStartup failed: %dn", WSAGetLastError());
      return 1;
   }

   
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock == INVALID_SOCKET) {
      printf("Socket creation failed: %dn", WSAGetLastError());
      WSACleanup();
      return 1;
   }

   
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
   serverAddr.sin_port = htons(PORT);

   while (true) {
   
      printf("Enter message: ");
      fgets(buffer, BUFFER_SIZE, stdin);

      
      buffer[strcspn(buffer, "\n")] = '\0';

      
      int sendResult = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
      if (sendResult == SOCKET_ERROR) {
         printf("Send failed: %dn", WSAGetLastError());
         break; 
      }

     
      int addrLen = sizeof(serverAddr);
      int bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&serverAddr, &addrLen);
      if (bytesReceived == SOCKET_ERROR) {
         printf("Receive failed: %dn", WSAGetLastError());
         break; 
      }

      buffer[bytesReceived] = '\0'; 
      printf("Received from server: %s\n", buffer);
   }

   closesocket(sock);
   WSACleanup();

   return 0;
}
