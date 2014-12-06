#include "ClientHelper.h"

BOOL ClientHelper::initSocket(int socketId, const addrinfo* address)
{
	if (connect(socketId, address->ai_addr, address->ai_addrlen) == -1)
	{
		closesocket(socketId);
		printError("connect");
		
		return false;
	}

	return true;
}