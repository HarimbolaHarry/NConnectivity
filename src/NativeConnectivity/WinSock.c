// © 2019 NIREX ALL RIGHTS RESERVED

#include "WinSock.h"

int InitiateWinSock(WSADATA* outData)
{
	WORD dllVersion = MAKEWORD(2, 1);
	return WSAStartup(dllVersion, outData);
}