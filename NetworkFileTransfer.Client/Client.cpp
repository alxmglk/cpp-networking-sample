#include "Client.h"


Client::Client(const char* serverHostname, const char* serverPort)
{
	this->serverHostname = serverHostname;
	this->serverPort = serverPort;

	this->helper.initializeWSA();
}

Client::~Client()
{
	this->helper.unitializeWSA();
}

void Client::run()
{
	const int bufferSize = 255;
	char buffer[bufferSize];
	SOCKET serverSocket = helper.getSocket(serverHostname, serverPort);
	if (serverSocket == -1)
	{
		helper.printError("can't connect to server");
		return;
	}

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

	closesocket(serverSocket);
}