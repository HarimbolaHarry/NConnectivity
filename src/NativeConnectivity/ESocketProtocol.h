// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _E_SOCKET_PROTOCOL_H_
#define _E_SOCKET_PROTOCOL_H_

namespace NConnectivity
{
	enum class ESocketProtocol
	{
		SP_ICMP = 1,
		SP_IGMP = 2,
		SP_RFCOMM = 3,
		SP_TCP= 6,
		SP_UDP = 17,
		SP_ICMPV6 = 58,
		SP_RM = 113
	};
}

#endif // !_E_SOCKET_PROTOCOL_H_
