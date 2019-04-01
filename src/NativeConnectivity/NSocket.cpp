// © 2019 NIREX ALL RIGHTS RESERVED

#include "NSocket.h"

namespace NConnectivity
{

	NSocket::NSocket(void)
	{
		sck = new SOCKET();

		AcceptRegistry = new SocketRegistry();
		ConnectRegistry = new SocketRegistry();
		SendRegistry = new SocketRegistry();
		SendToRegistry = new SocketRegistry();
		ReceiveRegistry = new SocketRegistry();
		ReceiveFromRegistry = new SocketRegistry();
		DisconnectRegistry = new SocketRegistry();
	}

	NSocket::~NSocket(void)
	{
		delete AcceptRegistry;
		delete ConnectRegistry;
		delete SendRegistry;
		delete SendToRegistry;
		delete ReceiveRegistry;
		delete ReceiveFromRegistry;
		delete DisconnectRegistry;
		
		delete sck;
	}

	void NSocket::Initialize(const IPEndPoint& inEndPoint, const SocketOptions& inOptions)
	{
		addr = inEndPoint.ip;
		port = inEndPoint.port;
		socket_type = (int)inOptions.type;
		protocol = (int)inOptions.protocol;
		flags = (int)inOptions.flags;


		endPoint.sin_addr.s_addr = inet_addr(inEndPoint.ip.c_str());
		endPoint.sin_port = htons(inEndPoint.port);
		endPoint.sin_family = AF_INET;

		addrLen = sizeof(endPoint);
		*sck = socket(endPoint.sin_family, socket_type, protocol);
	}

	int NSocket::Bind(void)
	{
		return bind(*sck, (SOCKADDR*)&endPoint, addrLen);
	}

	int NSocket::Listen(int backlog)
	{
		return listen(*sck, backlog);
	}

	SOCKET NSocket::Accept(void)
	{
		return accept(*sck, (SOCKADDR*)&endPoint, &addrLen);
	}

	void NSocket::BeginAccept(void)
	{
		std::thread t(&NSocket::HelperAcceptMethod, this);
		t.detach();
	}

	int NSocket::Connect(void)
	{
		return connect(*sck, (SOCKADDR*)&endPoint, addrLen);
	}

	void NSocket::BeginConnect(void)
	{
		std::thread t(&NSocket::HelperConnectMethod, this);
		
		t.detach();
	}

	int NSocket::Send(char* data, int data_size)
	{
		const char *data_ptr = (const char*)data;
		int bytes_sent;

		while (data_size > 0)
		{
			bytes_sent = send(*sck, data_ptr, data_size, flags);

			if (bytes_sent == SOCKET_ERROR)
			{
				return -1;
			}

			data_ptr += bytes_sent;
			data_size -= bytes_sent;
		}

		return 1;
	}

	void NSocket::BeginSend(char* data, int data_size)
	{
		std::thread t(&NSocket::HelperSendMethod, this, data, data_size);
		t.detach();
	}

	int NSocket::SendTo(char* data, int data_size)
	{
		int localAL = sizeof(endPoint);
		const char *data_ptr = (const char*)data;
		int bytes_sent;

		while (data_size > 0)
		{
			bytes_sent = sendto(*sck, data, data_size, flags, (SOCKADDR*)&endPoint, localAL);

			if (bytes_sent == SOCKET_ERROR)
			{
				return -1;
			}

			data_ptr += bytes_sent;
			data_size -= bytes_sent;
		}

		return 1;		
	}

	void NSocket::BeginSendTo(char* data, int data_size)
	{
		std::thread t(&NSocket::HelperSendToMethod, this, data, data_size);
		t.detach();
	}

	int NSocket::Receive(char* data, int data_size)
	{
		char *data_ptr = (char*)data;
		int bytes_recv;

		while (data_size > 0)
		{
			bytes_recv = recv(*sck, data_ptr, data_size, flags);
			if (bytes_recv <= 0)
			{
				return bytes_recv;
			}

			data_ptr += bytes_recv;
			data_size -= bytes_recv;
		}

		return 1;
	}

	void NSocket::BeginReceive(char* data, int data_size)
	{
		std::thread t(&NSocket::HelperReceiveMethod, this, data, data_size);
		t.detach();
	}

	int NSocket::ReceiveFrom(char* data, int data_size)
	{
		int localAL = sizeof(endPoint);
		char *data_ptr = (char*)data;
		int bytes_recv;

		while (data_size > 0)
		{
			bytes_recv = recvfrom(*sck, data, data_size, flags, (SOCKADDR*)&endPoint, &localAL);
			if (bytes_recv <= 0)
			{
				return bytes_recv;
			}

			data_ptr += bytes_recv;
			data_size -= bytes_recv;
		}

		return bytes_recv;
	}

	void NSocket::BeginReceiveFrom(char* data, int data_size)
	{
		std::thread t(&NSocket::HelperReceiveFromMethod, this, data, data_size);
		t.detach();
	}

	int NSocket::Disconnect(void)
	{
		return closesocket(*sck);
	}

	void NSocket::BeginDisconnect(void)
	{
		std::thread t(&NSocket::HelperDisconnectMethod, this);
		t.detach();
	}

	SOCKET* NSocket::GetSocket(void)
	{
		return sck;
	}

	SocketRegistry* NSocket::GetAcceptRegistry(void)
	{
		return AcceptRegistry;
	}

	SocketRegistry* NSocket::GetConnectRegistry(void)
	{
		return ConnectRegistry;
	}

	SocketRegistry* NSocket::GetSendRegistry(void)
	{
		return SendRegistry;
	}

	SocketRegistry* NSocket::GetSendToRegistry(void)
	{
		return SendToRegistry;
	}

	SocketRegistry* NSocket::GetReceiveRegistry(void)
	{
		return ReceiveRegistry;
	}

	SocketRegistry* NSocket::GetReceiveFromRegistry(void)
	{
		return ReceiveFromRegistry;
	}

	SocketRegistry* NSocket::GetDisconnectRegistry(void)
	{
		return DisconnectRegistry;
	}

	void NSocket::SetSocket(const SOCKET& sock)
	{
		if (sck)
		{
			delete sck;
		}
		sck = new SOCKET(sock);
	}

	void NSocket::HelperAcceptMethod(void)
	{
		SOCKET accepted = accept(*sck, (SOCKADDR*)&endPoint, &addrLen);
			
		if (accepted)
		{
			NSocket accepted_socket(*this);
			accepted_socket.SetSocket(accepted);

			std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(&accepted_socket, (int)accepted));
			AcceptRegistry->Run(this, args.get());
		}
	}

	void NSocket::HelperConnectMethod(void)
	{
		int result = connect(*sck, (SOCKADDR*)&endPoint, addrLen);
		std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(this, result));
		ConnectRegistry->Run(this, args.get());
	}

	void NSocket::HelperSendMethod(char* data, int data_size)
	{
		int result = 0;
		const char *data_ptr = (const char*)data;
		int bytes_sent;

		while (data_size > 0)
		{
			bytes_sent = send(*sck, data_ptr, data_size, flags);

			if (bytes_sent == SOCKET_ERROR)
			{
				result = -1;
				break;
			}

			data_ptr += bytes_sent;
			data_size -= bytes_sent;
		}

		result = 1;

		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, data, data_size, result));
		SendRegistry->Run(this, args.get());
	}

	void NSocket::HelperSendToMethod(char* data, int data_size)
	{
		int laddrLen = sizeof(endPoint);
		int result;
		const char *data_ptr = (const char*)data;
		int bytes_sent;

		while (data_size > 0)
		{
			bytes_sent = sendto(*sck, data, data_size, flags, (SOCKADDR*)&endPoint, laddrLen);

			if (bytes_sent == SOCKET_ERROR)
			{
				result = -1;
			}

			data_ptr += bytes_sent;
			data_size -= bytes_sent;
		}

		result = 1;


		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, data, data_size, result));;
		SendToRegistry->Run(this, args.get());
	}

	void NSocket::HelperReceiveMethod(char* data, int data_size)
	{
		char *data_ptr = (char*)data;
		int bytes_recv;
		int result;

		while (data_size > 0)
		{
			bytes_recv = recv(*sck, data_ptr, data_size, flags);
			if (bytes_recv <= 0)
			{
				result = bytes_recv;
			}

			data_ptr += bytes_recv;
			data_size -= bytes_recv;
		}

		result = bytes_recv;

		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, data, data_size, result));;
		ReceiveRegistry->Run(this, args.get());
	}

	void NSocket::HelperReceiveFromMethod(char * data, int data_size)
	{
		int localAL = sizeof(endPoint);
		char *data_ptr = (char*)data;
		int bytes_recv;
		int result = 0;

		while (data_size > 0)
		{
			bytes_recv = recvfrom(*sck, data, data_size, flags, (SOCKADDR*)&endPoint, &localAL);
			if (bytes_recv <= 0)
			{
				result = bytes_recv;
			}

			data_ptr += bytes_recv;
			data_size -= bytes_recv;
		}

		result = bytes_recv;

		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, data, data_size, result));;
		ReceiveFromRegistry->Run(this, args.get());
	}

	void NSocket::HelperDisconnectMethod(void)
	{
		int result = closesocket(*sck);
		std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(this, result));
		DisconnectRegistry->Run(this, args.get());
	}
}