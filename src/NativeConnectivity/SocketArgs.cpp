// © 2019 NIREX ALL RIGHTS RESERVED

#include "SocketArgs.h"

namespace NConnectivity
{
	SocketArgs::SocketArgs(Socket sock, int res)
		: connection(sock)
		, result(res)
	{
	}

	Socket SocketArgs::GetConnection(void)
	{
		return connection;
	}

	int SocketArgs::GetResult(void)
	{
		return result;
	}
}