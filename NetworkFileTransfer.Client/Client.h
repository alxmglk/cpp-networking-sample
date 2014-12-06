#pragma once
#include "ClientHelper.h"

class Client
{
public:
	Client(const char* serverHostname, const char* serverPort);
	~Client();
	void run();

private:
	const char* serverHostname;
	const char* serverPort;
	ClientHelper helper;
};