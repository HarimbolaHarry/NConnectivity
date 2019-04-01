// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _E_SOCKET_ADDRESS_FAMILY_H_
#define _E_SOCKET_ADDRESS_FAMILY_H_

namespace NConnectivity
{
	enum class ESocketAddressFamily
	{
		SAF_UNSPEC = 0,
		SAF_INET = 2,
		SAF_IPX = 6,
		SAF_APPLETALK = 16,
		SAF_NETBIOS = 17,
		SAF_INET6 = 23,
		SAF_IRDA = 26,
		SAF_BTH = 32,
	};
}

#endif // !_E_SOCKET_ADDRESS_FAMILY_H_
