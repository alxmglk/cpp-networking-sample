#pragma once
#include "NetworkHelper.h"

class ClientHelper : public NetworkHelper
{
protected:
	virtual BOOL initSocket(int socketId, const addrinfo* address);
};