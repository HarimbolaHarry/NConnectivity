// � 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_H_
#define _N_SOCKET_H_

#include "NSocketRegistry.h"
#include "NDefines.h"
#include "WinSock.h"
#include "TransferArgs.h"

#include <string>
#include <thread>
#include <memory>

namespace NConnectivity
{
	class NAPI NSocket final
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

		// Send data to the connected socket
		int Send(char* data, int dataLength);

		// Send data to the connected socket (ASYNC)
		void BeginSend(char* data, int dataLength);

		// Receive data from the connected socket
		int Receive(char* buffer, int length);

		// Receive data from the connected socket (ASYNC)
		void BeginReceive(char* buffer, int length);

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
		void SetNSocket(const SOCKET& sock);

		void HelperAcceptMethod(void);
		void HelperConnectMethod(void);
		void HelperSendMethod(char* data, int dataLength);
		void HelperReceiveMethod(char* buffer, int length);
		void HelperDisconnectMethod(void);

		SocketRegistry* AcceptRegistry;
		SocketRegistry* ConnectRegistry;
		SocketRegistry* SendRegistry;
		SocketRegistry* ReceiveRegistry;
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