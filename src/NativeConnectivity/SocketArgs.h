// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_ARGS_H_
#define _N_SOCKET_ARGS_H_

#include "NDefines.h"

namespace NConnectivity
{
	class NAPI Socket {};

	class NAPI SocketArgs
	{
	public:
		SocketArgs(Socket sock, int res);
		
		Socket GetConnection(void);
		
		int GetResult(void);

	private:
		Socket connection;
		int result;
	};
}

#endif // !_N_SOCKET_ARGS_H_