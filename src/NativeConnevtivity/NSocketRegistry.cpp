// © 2019 NIREX ALL RIGHTS RESERVED

#include "NSocketRegistry.h"

namespace NConnectivity
{
	SocketRegistry::SocketRegistry(void)
		: m_count(0)
	{
	}

	SocketRegistry::SocketRegistry(const SocketRegistry& nRegistery)
		: m_count(0)
	{
		for (size_t i = 0; i < nRegistery.m_registered.size(); i++)
		{
			this->Register(*nRegistery.m_registered[i]);
			_InterlockedIncrement(&m_count);
		}
	}

	SocketRegistry::SocketRegistry(SocketRegistry&& nRegistery)
		: m_count(0)
	{
		for (size_t i = 0; i < nRegistery.m_registered.size(); i++)
		{
			this->Register(*nRegistery.m_registered[i]);
			_InterlockedIncrement(&m_count);
		}
	}

	SocketRegistry::~SocketRegistry(void) {}

	long SocketRegistry::GetCount(void) const
	{
		return m_count;
	}

	SocketRegistry* SocketRegistry::operator()(Socket* sender, SocketArgs* args)
	{
		SocketAnnex current;
		for (size_t i = 0; i < m_registered.size(); i++)
		{
			if (m_registered[i] != nullptr)
			{
				current = *m_registered[i];
				current(sender, args);
			}
		}
		return this;
	}

	SocketRegistry* SocketRegistry::Run(Socket* sender, SocketArgs* args)
	{
		SocketAnnex current;
		for (size_t i = 0; i < m_registered.size(); i++)
		{
			if (m_registered[i] != nullptr)
			{
				current = *m_registered[i];
				current(sender, args);
			}
		}
		return this;
	}

	SocketRegistry* SocketRegistry::Register(SocketAnnex rhs)
	{
		m_registered.push_back(new SocketAnnex(rhs));
		_InterlockedIncrement(&m_count);
		return this;
	}

	SocketRegistry* SocketRegistry::UnRegister(SocketAnnex rhs)
	{
		for (size_t i = 0; i < m_registered.size(); i++)
		{
			if (m_registered[i] == &rhs)
			{
				m_registered[i] = nullptr;
				_InterlockedDecrement(&m_count);
			}
		}
		return this;
	}

	SocketRegistry* SocketRegistry::operator+=(SocketAnnex rhs)
	{
		m_registered.push_back(new SocketAnnex(rhs));
		_InterlockedIncrement(&m_count);
		return this;
	}

	SocketRegistry* SocketRegistry::operator-=(SocketAnnex rhs)
	{
		for (size_t i = 0; i < m_registered.size(); i++)
		{
			if (m_registered[i] == &rhs)
			{
				m_registered[i] = nullptr;
				_InterlockedDecrement(&m_count);
			}
		}
		return this;
	}

	std::vector<SocketAnnex*> SocketRegistry::Container(void) const
	{
		return m_registered;
	}
}
