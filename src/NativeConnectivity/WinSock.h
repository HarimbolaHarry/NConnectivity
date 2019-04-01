// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_WINSOCK_H_
#define _N_WINSOCK_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include "NWindows.h"

#define _INC_WINDOWS
#include <WinSock2.h>
#undef _INC_WINDOWS


int InitiateWinSock(WSADATA* outData);

#endif // !_N_WINSOCK_H_
