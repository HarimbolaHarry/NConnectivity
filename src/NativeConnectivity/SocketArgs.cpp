// © 2019 NIREX ALL RIGHTS RESERVED

#include "SocketArgs.h"
#include "Socket.h"

namespace NConnectivity
{
	SocketArgs::SocketArgs(NSocket* sock, int res)
		: connection(sock)
		, result(res)
	{
	}

	NSocket* SocketArgs::GetConnection(void)
	{
		return connection;
	}

	int SocketArgs::GetResult(void)
	{
		return result;
	}
}