// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_TRANSFER_ARGS_H_
#define _N_TRANSFER_ARGS_H_

#include "SocketArgs.h"

namespace NConnectivity
{
	class NAPI TransferArgs : public SocketArgs
	{
	public:
		TransferArgs(NSocket* sock, char* data, int dataLength, int result);

		char* GetData(void);

		int GetLength(void);
		
		char* GetProperData(void);

	private:
		char* data;
		char* properData;
		int dataLength;
	};
}

#endif // !_N_TRANSFER_ARGS_H_
