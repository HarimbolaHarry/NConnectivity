// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_OPTIONS_H_
#define _N_SOCKET_OPTIONS_H_

#include "ESocketType.h"
#include "ESocketProtocol.h"
#include "ESocketFlags.h"
#include "ESocketFamily.h"

namespace NConnectivity
{
	typedef struct
	{
		ESocketType type;
		ESocketProtocol protocol;
		ESocketFlags flags;
		ESocketAddressFamily addressFamily;

	} SocketOptions;
}

#endif // !_N_SOCKET_OPTIONS_H_
