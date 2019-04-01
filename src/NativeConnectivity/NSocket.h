// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_H_
#define _N_SOCKET_H_

#include "NSocketRegistry.h"
#include "NDefines.h"
#include "WinSock.h"
#include "TransferArgs.h"
#include "ISendable.h"

#include <string>
#include <thread>
#include <memory>

namespace NConnectivity
{
	class NAPI NSocket final : public ISendable
	{
	public:
		NSocket(const std::string& addr, int port, int socket_type, int protocol, int flags = 0);
		~NSocket(void);

		// Bind socket to the EndPoint
		int Bind(void);

		// Begin Listening on the EndPoint
		int Listen(int backlog = SOMAXCONN);
		
		// Accept an incomming connection
		SOCKET Accept(void);

		// Accept an incomming connection (ASYNC)
		void BeginAccept(void);

		// Connect to a socket which is on listening state
		int Connect(void);

		// Connect to a socket which is on listening state (ASYNC)
		void BeginConnect(void);

		// Send all the data to the connected socket
		int Send(char* data, int data_size);

		// Send all the data to the connected socket (ASYNC)
		void BeginSend(char* data, int data_size);

		// Send all the data to the given endpoint (datagram)
		int SendTo(char* data, int data_size, const std::string& addr, int port);

		// Send all the data to the given endpoint (datagram) (ASYNC)
		void BeginSendTo(char* data, int data_size, const std::string& addr, int port);

		// Receive all the data from the connected socket
		int Receive(char* data, int data_size);

		// Receive all the data from the connected socket (ASYNC)
		void BeginReceive(char* data, int data_size);

		// Receive all the data comming from the given endpoint (datagram)
		int ReceiveFrom(char* data, int data_size, const std::string& addr, int port);

		// Receive all the data comming from the given endpoint (datagram) (ASYNC)
		void BeginReceiveFrom(char* data, int data_size, const std::string& addr, int port);

		// Disconnect the main socket
		int Disconnect(void);

		// Disconnect the main socket (ASYNC)
		void BeginDisconnect(void);

		SOCKET* GetNSocket(void);

		SocketRegistry* GetAcceptRegistry(void);

		SocketRegistry* GetConnectRegistry(void);
		
		SocketRegistry* GetSendRegistry(void);
		
		SocketRegistry* GetReceiveRegistry(void);
		
		SocketRegistry* GetDisconnectRegistry(void);

	private:
		void SetSocket(const SOCKET& sock);

		void HelperAcceptMethod(void);
		void HelperConnectMethod(void);
		void HelperSendMethod(char* data, int data_size);
		void HelperSendToMethod(char* data, int data_size, const std::string& addr, int port);
		void HelperReceiveMethod(char* data, int data_size);
		void HelperReceiveFromMethod(char* data, int data_size, const std::string& addr, int port);
		void HelperDisconnectMethod(void);

		SocketRegistry* AcceptRegistry;
		SocketRegistry* ConnectRegistry;
		SocketRegistry* SendRegistry;
		SocketRegistry* SendToRegistry;
		SocketRegistry* ReceiveRegistry;
		SocketRegistry* ReceiveFromRegistry;
		SocketRegistry* DisconnectRegistry;

		int flags;
		int addrLen;
		SOCKADDR_IN endPoint;
		SOCKET* sck;

		// Raw data
		std::string addr;
		int port;
		int socket_type;
		int protocol;

	};
}
#endif // !_N_SOCKET_H_
