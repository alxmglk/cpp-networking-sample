#pragma once
#include "../NetworkFileTransfer.Core/NetworkHelper.h"

class ServerHelper : public NetworkHelper
{
protected:
	virtual BOOL initSocket(int socketId, const addrinfo* address);
};