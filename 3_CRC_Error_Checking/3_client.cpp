#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
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


int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) 
	{
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
	{
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) 
	{
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    //crc client
    while(1)
    {
    	char dataword[30], generator[30],cd[30];
		string remainder, codeword; 
    	cout<<"Enter dataword: ";
    	cin>>dataword;
    	cout<<"Enter generator: ";
    	cin>>generator;
    	CRC(dataword, generator,&remainder, &codeword);
    	cout<<"True codeword: "<<codeword<<endl;
    	cout<<"Enter codeword to sent to server: ";
    	cin>>cd;
    	send(ConnectSocket,cd,sizeof(cd),0);
    	send(ConnectSocket,generator,sizeof(generator),0);
    	cout<<"Codeword sending to server";
    	memset(recvbuf,0,DEFAULT_BUFLEN);
    	iResult=recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
    	if(iResult>0)
    		printf("\nServer : %s",recvbuf);
    	break;
    		
	}
    	

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
	{
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
	printf("\n---Connection closed---");
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
