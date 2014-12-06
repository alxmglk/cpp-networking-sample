#include "ServerHelper.h"

BOOL ServerHelper::initSocket(int socketId, const addrinfo* address)
{
	char optValue = 1;
	if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(char)) == -1)
	{
		printError("setsockopt");
		return false;
	}

	if (bind(socketId, address->ai_addr, address->ai_addrlen) == -1)
	{
		closesocket(socketId);
		printError("bind");
		return false;
	}

	return true;
}