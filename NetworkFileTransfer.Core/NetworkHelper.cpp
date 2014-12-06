#include "NetworkHelper.h"

BOOL NetworkHelper::isWSAInitialized = FALSE;

void NetworkHelper::initializeWSA()
{
	if (isWSAInitialized)
	{
		return;
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		printError("WSAStartup failed");
		exit(1);
	}

	isWSAInitialized = true;
}

void NetworkHelper::unitializeWSA()
{
	if (isWSAInitialized)
	{
		WSACleanup();
	}
}

SOCKET NetworkHelper::getSocket(const char* hostname, const char* port)
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

		if (!initSocket(socketId, p))
		{
			printError("getSocket.initSocket");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		printError("getSocket");
		return -1;
	}

	printSocketAddress(p);
	freeaddrinfo(address);

	return socketId;
}

BOOL NetworkHelper::initSocket(int socketId, const addrinfo* address)
{
	return TRUE;
}

void NetworkHelper::printError(const char* message)
{
	fprintf(stderr, "Error: %s\n", message);
}

void NetworkHelper::printSocketAddress(addrinfo* addr)
{
	char s[INET6_ADDRSTRLEN];
	inet_ntop(addr->ai_family, convertSocketAddress(addr->ai_addr), s, sizeof(s));

	printf("Socket address %s\n", s);
}

void *NetworkHelper::convertSocketAddress(sockaddr* addr)
{
	if (addr->sa_family == AF_INET)
	{
		return &(((sockaddr_in*)addr)->sin_addr);
	}

	return &(((sockaddr_in6*)addr)->sin6_addr);
}