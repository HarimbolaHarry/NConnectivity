// © 2019 NIREX ALL RIGHTS RESERVED

#include "Socket.h"

namespace NConnectivity
{
	Socket::Socket(const std::string& addr, int port, int socket_type, int protocol, int flags)
		: addr(addr)
		, port(port)
		, socket_type(socket_type)
		, protocol(protocol)
		, flags(flags)
	{
		sck = new SOCKET();

		endPoint.sin_addr.s_addr = inet_addr(addr.c_str());
		endPoint.sin_port = htons(port);
		// For UDP & TCP Sockets
		endPoint.sin_family = AF_INET;

		addrLen = sizeof(endPoint);
		*sck = socket(endPoint.sin_family, socket_type, protocol);

		AcceptRegistry = new SocketRegistry();
		ConnectRegistry = new SocketRegistry();
		SendRegistry = new SocketRegistry();
		ReceiveRegistry = new SocketRegistry();
		DisconnectRegistry = new SocketRegistry();
	}

	Socket::~Socket(void)
	{
		delete AcceptRegistry;
		delete ConnectRegistry;
		delete SendRegistry;
		delete ReceiveRegistry;
		delete DisconnectRegistry;

		delete sck;
	}

	int Socket::Bind(void)
	{
		return bind(*sck, (SOCKADDR*)&endPoint, addrLen);
	}

	int Socket::Listen(int backlog)
	{
		return listen(*sck, backlog);
	}

	SOCKET Socket::Accept(void)
	{
		return accept(*sck, (SOCKADDR*)&endPoint, &addrLen);
	}

	void Socket::BeginAccept(void)
	{
		std::thread t(&Socket::AcceptMethod, this);
		t.detach();
	}

	int Socket::Connect(void)
	{
		return connect(*sck, (SOCKADDR*)&endPoint, addrLen);
	}

	void Socket::BeginConnect(void)
	{
		std::thread t(&Socket::ConnectMethod, this);
		t.detach();
	}

	int Socket::Send(char* data, int dataLength)
	{
		return send(*sck, data, dataLength, flags);
	}

	void Socket::BeginSend(char* data, int dataLength)
	{
		std::thread t(&Socket::SendMethod, this, data, dataLength);
		t.detach();
	}

	int Socket::Receive(char* buffer, int length)
	{
		return recv(*sck, buffer, length, flags);
	}

	void Socket::BeginReceive(char* buffer, int length)
	{
		std::thread t(&Socket::ReceiveMethod, this, buffer, length);
		t.detach();
	}

	int Socket::Disconnect(void)
	{
		return closesocket(*sck);
	}

	void Socket::BeginDisconnect(void)
	{
		std::thread t(&Socket::DisconnectMethod, this);
		t.detach();
	}

	SOCKET* Socket::GetSocket(void)
	{
		return sck;
	}

	SocketRegistry* Socket::GetAcceptRegistry(void)
	{
		return AcceptRegistry;
	}

	SocketRegistry* Socket::GetConnectRegistry(void)
	{
		return ConnectRegistry;
	}

	SocketRegistry* Socket::GetSendRegistry(void)
	{
		return SendRegistry;
	}

	SocketRegistry* Socket::GetReceiveRegistry(void)
	{
		return ReceiveRegistry;
	}

	SocketRegistry* Socket::GetDisconnectRegistry(void)
	{
		return DisconnectRegistry;
	}

	void Socket::SetSocket(const SOCKET& sock)
	{
		if (sck)
		{
			delete sck;
		}
		sck = new SOCKET(sock);
	}

	void Socket::AcceptMethod(void)
	{
		SOCKET accepted = accept(*sck, (SOCKADDR*)&endPoint, &addrLen);
			
		if (accepted)
		{
			int result = 0;
		
			Socket accepted_socket(*this);
			accepted_socket.SetSocket(accepted);

			std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(accepted_socket, result));
			AcceptRegistry->Run(this, args.get());
		}
	}

	void Socket::ConnectMethod(void)
	{
		int result = connect(*sck, (SOCKADDR*)&endPoint, addrLen);
		std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(*this, result));
		ConnectRegistry->Run(this, args.get());
	}

	void Socket::SendMethod(char* data, int dataLength)
	{
		int result = send(*sck, data, dataLength, flags);
		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(*this, data, dataLength, result));
		SendRegistry->Run(this, args.get());
	}

	void Socket::ReceiveMethod(char* buffer, int length)
	{
		int result = recv(*sck, buffer, length, flags);
		std::unique_ptr<TransferArgs> args = std::make_unique<TransferArgs>(TransferArgs(*this, buffer, length, result));;
		ReceiveRegistry->Run(this, args.get());
	}

	void Socket::DisconnectMethod(void)
	{
		int result = closesocket(*sck);
		std::unique_ptr<SocketArgs> args = std::make_unique<SocketArgs>(SocketArgs(*this, result));
		DisconnectRegistry->Run(this, args.get());
	}
}