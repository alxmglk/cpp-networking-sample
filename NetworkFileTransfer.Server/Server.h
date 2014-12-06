#pragma once
#include "ServerHelper.h"

class Server
{
public:
	Server(const char* hostname, const char* port, int queueSize);
	~Server();
	void run();

private:
	const char* hostname;
	const char* port;
	int queueSize;

	ServerHelper helper;
};