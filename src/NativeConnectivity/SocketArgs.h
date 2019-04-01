// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_ARGS_H_
#define _N_SOCKET_ARGS_H_

#include "NDefines.h"

namespace NConnectivity
{
	class NSocket;

	class NAPI SocketArgs
	{
	public:
		SocketArgs(NSocket* sock, int res);
		
		NSocket* GetConnection(void);
		
		int GetResult(void);

	private:
		NSocket* connection;
		int result;
	};
}

#endif // !_N_SOCKET_ARGS_H_