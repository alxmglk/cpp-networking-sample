#pragma once
#include <stdio.h>
#include <WS2tcpip.h>
#include <windows.h>
#include "ExpressionEvaluator.h"

const char* host = "127.0.0.1";
const char* port = "33444";
const int queueSize = 10;

void WSAInitialize();
void printError(const char* message);
SOCKET getSocket(const char* host, const char* port);
void run(SOCKET serverSocket);

typedef struct {
	DWORD threadId;
	HANDLE threadHandle;
	SOCKET socket;
} TDATA;

int main()
{
	WSAInitialize();

	SOCKET serverSocket = getSocket(host, port);
	if (serverSocket == -1)
	{
		printError("can't run server");
		return 1;
	}

	if (listen(serverSocket, queueSize) == -1)
	{
		printError("listen");
		return 1;
	}

	run(serverSocket);

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}

void printError(const char* message)
{
	fprintf(stderr, "Error: %s\n", message);
}

void WSAInitialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		printError("WSAStartup failed");
		exit(1);
	}
}

SOCKET getSocket(const char* hostname, const char* port)
{
	int socketId;
	addrinfo *address, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int result;
	if ((result = getaddrinfo(hostname, port, &hints, &address)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
		return -1;
	}

	addrinfo *p;
	for (p = address; p != NULL; p = p->ai_next)
	{
		if ((socketId = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			printError("getSocket.socket");
			continue;
		}

		char optValue = 1;
		if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(char)) == -1)
		{
			printError("setsockopt");
			continue;
		}

		if (bind(socketId, address->ai_addr, address->ai_addrlen) == -1)
		{
			closesocket(socketId);
			printError("bind");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		printError("getSocket");
		return -1;
	}

	freeaddrinfo(address);

	return socketId;
}

// Parse input: divide on two parts - expression and operation.
void parseMessage(const char* message, char* expression, char* operation)
{
	int messageIndex = 0;

	// read expression
	int i = 0;
	for (; message[messageIndex] != '|' && message[messageIndex] != '\0'; ++messageIndex, ++i)
	{
		expression[i] = message[messageIndex];
	}

	expression[i] = '\0';

	// read operation
	++messageIndex;

	i = 0;
	for (; message[messageIndex] != '\0'; ++i, ++messageIndex)
	{
		operation[i] = message[messageIndex];
	}

	operation[i] = '\0';
}

DWORD WINAPI handleRequest(LPVOID param)
{
	TDATA* data = (TDATA*)param;

	int recvSize;
	const int bufferSize = 255;
	char buffer[bufferSize];

	if ((recvSize = recv(data->socket, buffer, bufferSize - 1, 0)) != -1)
	{
		buffer[recvSize] = '\0';
		
		char expression[255];
		char operation[15];

		parseMessage(buffer, expression, operation);

		ExpressionEvaluator evaluator;
		char* result = evaluator.evaluate(expression, operation);

		if (send(data->socket, result, strlen(result), 0) == -1)
		{
			printError("send");
		}

		delete[] result;
	}

	closesocket(data->socket);

	CloseHandle(data->threadHandle);

	return 0;
}

void run(SOCKET serverSocket)
{
	SOCKET socket;
	sockaddr address;
	socklen_t addressSize;

	char saddress[INET_ADDRSTRLEN];

	printf("server: waiting for connections...\n");

	while (1)
	{
		addressSize = sizeof(address);

		// Wating for client connection
		socket = accept(serverSocket, &address, &addressSize);
		if (socket == -1)
		{
			printError("accept");
			continue;
		}

		inet_ntop(address.sa_family, &((sockaddr_in*)&address)->sin_addr, saddress, sizeof(saddress));
		printf("server: got connection from %s\n", saddress);

		TDATA threadData;
		threadData.socket = socket;

		HANDLE thread = CreateThread(NULL, 0, handleRequest, &threadData, 0, &(threadData.threadId));

		threadData.threadHandle = thread;
	}
}