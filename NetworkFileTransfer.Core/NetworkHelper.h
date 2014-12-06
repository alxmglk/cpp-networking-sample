#pragma once
#include <stdio.h>
#include <WS2tcpip.h>
#include <windows.h>

class NetworkHelper
{
public:
	void initializeWSA();
	void unitializeWSA();
	SOCKET getSocket(const char* hostname, const char* port);
	static void *convertSocketAddress(sockaddr* addr);
	void printError(const char* message);

protected:
	virtual BOOL initSocket(int socketId, const addrinfo* address);
	
private:
	static BOOL isWSAInitialized;
	static void printSocketAddress(addrinfo* adrr);
};