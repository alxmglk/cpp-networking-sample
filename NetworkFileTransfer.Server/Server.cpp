#include "Server.h"

Server::Server(const char* hostname, const char* port, int queueSize)
{
	this->hostname = hostname;
	this->port = port;
	this->queueSize = queueSize;

	this->helper.initializeWSA();
}

Server::~Server()
{
	this->helper.unitializeWSA();
}

void Server::run()
{
	SOCKET serverSocket = helper.getSocket(this->hostname, this->port);
	if (serverSocket == -1)
	{
		helper.printError("can't run server");
		return;
	}

	if (listen(serverSocket, queueSize) == -1)
	{
		helper.printError("listen");
		return;
	}

	printf("server: waiting for connections...\n");

	SOCKET socket;
	sockaddr_storage address;
	socklen_t addressSize;

	int recvSize;
	const int bufferSize = 255;
	char buffer[bufferSize];
	char saddress[INET6_ADDRSTRLEN];

	while (1)
	{
		addressSize = sizeof(address);
		socket = accept(serverSocket, (sockaddr*)&address, &addressSize);
		if (socket == -1)
		{
			helper.printError("accept");
			continue;
		}

		inet_ntop(address.ss_family, helper.convertSocketAddress((sockaddr*)&address), saddress, sizeof(saddress));
		printf("server: got connection from %s\n", saddress);

		while ((recvSize = recv(socket, buffer, bufferSize - 1, 0)) != -1)
		{
			buffer[recvSize] = '\0';

			printf("Received from client: %s\n", buffer);

			if (send(socket, buffer, recvSize, 0) == -1)
			{
				helper.printError("send");
			}

			if (strcmp(buffer, "end") == 0)
			{
				printf("Closing connection with client: %s\n", saddress);
				closesocket(socket);
				break;
			}
		}
	}
}