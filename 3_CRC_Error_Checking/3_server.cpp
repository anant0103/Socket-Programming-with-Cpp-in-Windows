#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <iostream>

using namespace std;


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

string toBin(long long int num){ 
    string bin = ""; 
    while (num){ 
        if (num & 1) 
            bin = "1" + bin; 
        else
            bin = "0" + bin; 
        num = num>>1; 
    } 
    return bin; 
} 

long long int toDec(string bin){ 
    long long int num = 0; 
    for (int i=0; i<bin.length(); i++){ 
        if (bin.at(i)=='1') 
            num += 1 << (bin.length() - i - 1); 
    } 
    return num; 
}


void CRC(string dataword, string generator, string* re,string* co) { 
    int l_gen = generator.length(); 
    long long int gen = toDec(generator); 
  
    long long int dword = toDec(dataword); 
  
     // append 0s to dividend 
    long long int dividend = dword << (l_gen-1);        
  
    // shft specifies the no. of least  
    // significant bits not being XORed 
    int shft = (int) ceill(log2l(dividend+1)) - l_gen;   
    long long int rem; 
  
    while ((dividend >= gen) || (shft >= 0)){ 
  
        // bitwise XOR the MSBs of dividend with generator 
        // replace the operated MSBs from the dividend with 
        // remainder generated  
        rem = (dividend >> shft) ^ gen;                 
        dividend = (dividend & ((1 << shft) - 1)) | (rem << shft); 
  
        // change shft variable 
        shft = (int) ceill(log2l(dividend + 1)) - l_gen; 
    } 
  
    // finally, AND the initial dividend with the remainder (=dividend) 
    long long int codeword = (dword << (l_gen - 1)) | dividend; 
    *re=toBin(dividend);
    *co=toBin(codeword);
} 


int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) 
	{
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) 
	{
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
	{
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
	{
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) 
	{
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

	//crc server
    char codeword[30],generator[30];
    string remainder,ans;
    char a1[10]="NO ERROR",a2[10]="ERROR";
    recv(ClientSocket, codeword, 30, 0);
	recv(ClientSocket, generator, 30, 0);
	printf("Codeword received.\n");
	printf("Generator received.\n");
	printf("CRC checking...\n");
	CRC(codeword, generator,&remainder, &ans); 
	if(remainder=="")
	{
		cout<<"Message "<<a1<<" sending to client";
		send(ClientSocket, a1, (int)strlen(a1), 0 );
	}
	else
	{
		cout<<"Message "<<a2<<" sending to client";
		send(ClientSocket, a2, (int)strlen(a2), 0 );
	}



    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
	printf("\n---Connection closed---");
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

