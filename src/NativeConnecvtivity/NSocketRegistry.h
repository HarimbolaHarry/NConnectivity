// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_REGISTRY_H_
#define _N_SOCKET_REGISTRY_H_

#include <vector>
#include <functional>
#include "NDefines.h"
#include "SocketArgs.h"

namespace NConnectivity
{
	class Socket;

	typedef std::function<void(Socket* sender, SocketArgs* args)> SocketAnnex;

	class NAPI SocketRegistry final
	{
	public:
		SocketRegistry(void);
		SocketRegistry(const SocketRegistry& nRegistery);
		SocketRegistry(SocketRegistry&& nRegistery);
		~SocketRegistry(void);

		long GetCount(void) const;

		SocketRegistry* operator()(Socket* sender, SocketArgs* args);

		SocketRegistry* Run(Socket* sender, SocketArgs* args);

		SocketRegistry* Register(SocketAnnex rhs);

		SocketRegistry* UnRegister(SocketAnnex rhs);

		SocketRegistry* operator+=(SocketAnnex rhs);

		SocketRegistry* operator-=(SocketAnnex rhs);

		std::vector<SocketAnnex*> Container(void) const;

	private:
		volatile long m_count;
		std::vector<SocketAnnex*> m_registered;
	};
}
#endif // !_N_SOCKET_REGISTRY_H_
