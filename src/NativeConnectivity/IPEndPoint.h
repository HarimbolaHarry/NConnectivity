// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_IP_ENDPOINT_H_
#define _N_IP_ENDPOINT_H_

#include <string>

namespace NConnectivity
{
	typedef struct
	{
		std::string ip;
		int port;

	} IPEndPoint;
}

#endif // !_N_IP_ENDPOINT_H_
