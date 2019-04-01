// © 2019 NIREX ALL RIGHTS RESERVED

#include "TransferArgs.h"
#include "Socket.h"

namespace NConnectivity
{
	TransferArgs::TransferArgs(NSocket* sock, char* data, int dataLength, int result)
		: SocketArgs(sock, result)
		, data(data)
		, dataLength(dataLength)
	{
		properData = new char[dataLength];

		for (int i = 0; i < dataLength; i++)
		{
			properData[i] = data[i];
		}
	}

	char* TransferArgs::GetData(void)
	{
		return data;
	}

	int TransferArgs::GetLength(void)
	{
		return dataLength;
	}

	char* TransferArgs::GetProperData(void)
	{
		return properData;
	}
}
