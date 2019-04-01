// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _E_SOCKET_FLAGS_H_
#define _E_SOCKET_FLAGS_H_

namespace NConnectivity
{
	enum class ESocketFlags
	{
		SF_None = 0,
		SF_OutOfBand = 1,
		SF_Peek = 2,
		SF_DontRoute = 4,
		SF_MaxIOVectorLength = 16,
		SF_Truncated = 256,
		SF_ControlDataTruncated = 512,
		SF_Broadcast = 1024,
		SF_Multicast = 2048,
		SF_Partial = 32768
	};
}

#endif // !_E_SOCKET_FLAGS_H_
