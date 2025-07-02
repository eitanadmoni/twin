#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "Exception.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")


enum returnValue {
    SUCCESS,
    FAILURE,
};


const int DEFAULT_BUFLEN = 512;


using std::string;
using std::cerr;
using std::endl;

/*
* @brief finction to clean buffer and fill it with null terminator
* @param buffer a array to fill with null terminator
* @param size the size of the array
* @param size the size of the array
*/
void cleanBuf(char* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = '\0';
    }
}


/*
* @brief open a server that listen for ping and return pong
* @return value that tell if the program succeeded
*/
int pingServerSocket(PCSTR port) {
    WSADATA wsaData;
    int iResult;
    int* lenPointer;
    int messageLen;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvBuf[DEFAULT_BUFLEN] = { '\0' };
    const string pong = "PONG";
    const string ping = "PING";
    int recvBuflen = DEFAULT_BUFLEN;

    try {
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            throw Exception("WSAStartup failed");
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, port, &hints, &result);
        if (iResult != 0) {
            WSACleanup();
            throw Exception("getaddrinfo failed");
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            freeaddrinfo(result);
            WSACleanup();
            throw Exception("Listen socket failed");
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            throw Exception("bind failed");
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            closesocket(ListenSocket);
            WSACleanup();
            throw Exception("listen failed");
        }

        while (true) {
            // Accept a client socket
            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                closesocket(ListenSocket);
                throw Exception("accept failed");
            }


            // Receive until the peer shuts down the connection
            do {
                cleanBuf(recvBuf, std::size(recvBuf));
                iResult = recv(ClientSocket, recvBuf, 4, 0);
                if (iResult > 0) {
                    lenPointer = (int*)recvBuf;
                    messageLen = lenPointer[0];
                    cleanBuf(recvBuf, std::size(recvBuf));
                    iResult = recv(ClientSocket, recvBuf, messageLen, 0);
                    if (ping == (string)recvBuf) {
                        printf("PING received\n");
                        iSendResult = send(ClientSocket, pong.c_str(), iResult, 0);
                        if (iSendResult == SOCKET_ERROR) {
                            printf("send failed with error: %d\n", WSAGetLastError());
                        }
                    }
                    else {
                        printf("Unknown message\n");
                        iSendResult = send(ClientSocket, "Unknown message", std::size("Unknown message"), 0);
                        if (iSendResult == SOCKET_ERROR) {
                            printf("send failed with error: %d\n", WSAGetLastError());
                        }
                    }

                }

                else if (iResult == 0) {
                    printf("Connection closing...\n");
                }
                else {
                    printf("recv failed with error: %d\n", WSAGetLastError());
                    break;
                }
            } while (iResult > 0);

            // shutdown the connection since we're done
            iResult = shutdown(ClientSocket, SD_SEND);
            if (iResult == SOCKET_ERROR) {
                closesocket(ClientSocket);
                throw Exception("shutdown failed");
            }

            // cleanup
            closesocket(ClientSocket);
        }
    }

    catch (const Exception& e) {
        closesocket(ListenSocket);
        WSACleanup();
        cerr << e.getError() << endl;
        return FAILURE;
    }
    closesocket(ListenSocket);
    WSACleanup();
    return SUCCESS;

    return SUCCESS;
}


int __cdecl main(int argc, char** argv){
    if (argc != 2) {
        cerr << "Number of arguments need to be exactly 1 - port to listen at" << endl;
        return FAILURE;
    }
    PCSTR port = argv[1];
    return pingServerSocket(port);
}
