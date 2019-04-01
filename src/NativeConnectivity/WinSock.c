// © 2019 NIREX ALL RIGHTS RESERVED

#include "WinSock.h"

int InitiateWinSock(WSADATA* outData)
{
	WORD dllVersion = MAKEWORD(2, 1);
	int res = WSAStartup(dllVersion, outData);
	if (res)
	{
		bIsInitialized = 1;
		return res;
	}
	bIsInitialized = 0;
	return res;
}
