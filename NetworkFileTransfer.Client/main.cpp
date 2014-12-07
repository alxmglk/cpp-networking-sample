#pragma once
#include <stdio.h>
#include <WS2tcpip.h>
#include <windows.h>

const char* host = "127.0.0.1";
const char* port = "33444";

void WSAInitialize();
void printError(const char* message);
SOCKET getSocket(const char* host, const char* port);
void run(SOCKET serverSocket);

int main()
{
	WSAInitialize();

	SOCKET serverSocket = getSocket(host, port);
	if (serverSocket == -1)
	{
		printError("can't connect to server");
		return;
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

		if (connect(socketId, address->ai_addr, address->ai_addrlen) == -1)
		{
			closesocket(socketId);
			printError("connect");

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

void run(SOCKET serverSocket)
{
	const int bufferSize = 255;
	char buffer[bufferSize];

	while (1)
	{
		printf("\n\nEnter message to send: ");

		scanf("%s", buffer);
		int messageLength = strlen(buffer);

		if (send(serverSocket, buffer, messageLength, 0) == -1)
		{
			break;
		}

		if ((messageLength = recv(serverSocket, buffer, bufferSize - 1, 0)) == -1)
		{
			break;
		}

		buffer[messageLength] = '\0';

		printf("Received message: %s", buffer);
	}

	printf("\nServer has closed the connection\n");
}