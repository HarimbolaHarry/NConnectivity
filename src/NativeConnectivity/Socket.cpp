// © 2019 NIREX ALL RIGHTS RESERVED

#include "Socket.h"

namespace NConnectivity
{
	NSocket::NSocket(const std::string& addr, int port, int socket_type, int protocol, int flags)
		: addr(addr)
		, port(port)
		, socket_type(socket_type)
		, protocol(protocol)
		, flags(flags)
	{
		sck = new SOCKET();

		endPoint.sin_addr.s_addr = inet_addr(addr.c_str());
		endPoint.sin_port = htons(port);
		// For UDP & TCP NSockets
		endPoint.sin_family = AF_INET;

		addrLen = sizeof(endPoint);
		*sck = socket(endPoint.sin_family, socket_type, protocol);

		AcceptRegistry = new SocketRegistry();
		ConnectRegistry = new SocketRegistry();
		SendRegistry = new SocketRegistry();
		ReceiveRegistry = new SocketRegistry();
		DisconnectRegistry = new SocketRegistry();
	}

	NSocket::~NSocket(void)
	{
		delete AcceptRegistry;
		delete ConnectRegistry;
		delete SendRegistry;
		delete ReceiveRegistry;
		delete DisconnectRegistry;

		delete sck;
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

	int NSocket::Send(char* data, int dataLength)
	{
		return send(*sck, data, dataLength, flags);
	}

	void NSocket::BeginSend(char* data, int dataLength)
	{
		std::thread t(&NSocket::HelperSendMethod, this, data, dataLength);
		t.detach();
	}

	int NSocket::Receive(char* buffer, int length)
	{
		return recv(*sck, buffer, length, flags);
	}

	void NSocket::BeginReceive(char* buffer, int length)
	{
		std::thread t(&NSocket::HelperReceiveMethod, this, buffer, length);
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

	SOCKET* NSocket::GetNSocket(void)
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

	SocketRegistry* NSocket::GetReceiveRegistry(void)
	{
		return ReceiveRegistry;
	}

	SocketRegistry* NSocket::GetDisconnectRegistry(void)
	{
		return DisconnectRegistry;
	}

	void NSocket::SetNSocket(const SOCKET& sock)
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
			accepted_socket.SetNSocket(accepted);

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

	void NSocket::HelperSendMethod(char* data, int dataLength)
	{
		int result = send(*sck, data, dataLength, flags);
		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, data, dataLength, result));
		SendRegistry->Run(this, args.get());
	}

	void NSocket::HelperReceiveMethod(char* buffer, int length)
	{
		int result = recv(*sck, buffer, length, flags);
		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(this, buffer, length, result));;
		ReceiveRegistry->Run(this, args.get());
	}

	void NSocket::HelperDisconnectMethod(void)
	{
		int result = closesocket(*sck);
		std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(this, result));
		DisconnectRegistry->Run(this, args.get());
	}
}