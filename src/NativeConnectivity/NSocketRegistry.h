// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_REGISTRY_H_
#define _N_SOCKET_REGISTRY_H_

#include <vector>
#include <functional>

#include "NDefines.h"
#include "SocketArgs.h"
#include "ISendable.h"

namespace NConnectivity
{
	typedef std::function<void(ISendable* sender, SocketArgs* args)> SendableAnnex;

	class NAPI SocketRegistry final
	{
	public:
		SocketRegistry(void);
		SocketRegistry(const SocketRegistry& nRegistery);
		SocketRegistry(SocketRegistry&& nRegistery);
		~SocketRegistry(void);

		long GetCount(void) const;

		SocketRegistry* operator()(ISendable* sender, SocketArgs* args);

		SocketRegistry* Run(ISendable* sender, SocketArgs* args);

		SocketRegistry* Register(SendableAnnex rhs);

		SocketRegistry* UnRegister(SendableAnnex rhs);

		SocketRegistry* operator+=(SendableAnnex rhs);

		SocketRegistry* operator-=(SendableAnnex rhs);

		std::vector<SendableAnnex*> Container(void) const;

	private:
		volatile long m_count;
		std::vector<SendableAnnex*> m_registered;
	};
}
#endif // !_N_SOCKET_REGISTRY_H_
