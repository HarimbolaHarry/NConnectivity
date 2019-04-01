// © 2019 NIREX ALL RIGHTS RESERVED

#ifndef _N_SOCKET_H_
#define _N_SOCKET_H_

#include "NSocketRegistry.h"
#include "NDefines.h"
#include "WinSock.h"
#include "TransferArgs.h"
#include "ISendable.h"
#include "IPEndPoint.h"
#include "SocketOptions.h"

#include <string>
#include <thread>
#include <memory>

namespace NConnectivity
{
	class NAPI NSocket final : public ISendable
	{
	public:
		NSocket(void);
		~NSocket(void);

		// TCP: IPEndPoint will be used to act as the binding address as the server or the address to connect to as the client.
		// UDP: IPEndPoint will be used to act as the endpoint to send data to or to receive data from.
		void Initialize(const IPEndPoint& inEndPoint, const SocketOptions& inOptions);

		// Bind socket to the EndPoint
		int Bind(void);

		// Begin Listening on the EndPoint
		int Listen(int backlog = SOMAXCONN);
		
		// Accept an incomming connection
		// Parameters are optional Here
		SOCKET Accept(SOCKADDR_IN* address, int* len);

		// Accept an incomming connection (ASYNC)
		// Accept Parameters can be accessed in the raised event's Arguments
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
		int SendTo(char* data, int data_size);

		// Send all the data to the given endpoint (datagram) (ASYNC)
		void BeginSendTo(char* data, int data_size);

		// Receive all the data from the connected socket
		int Receive(char* data, int data_size);

		// Receive all the data from the connected socket (ASYNC)
		void BeginReceive(char* data, int data_size);

		// Receive all the data comming from the given endpoint (datagram)
		int ReceiveFrom(char* data, int data_size);

		// Receive all the data comming from the given endpoint (datagram) (ASYNC)
		void BeginReceiveFrom(char* data, int data_size);

		// Disconnect the main socket
		int Disconnect(void);

		// Disconnect the main socket (ASYNC)
		void BeginDisconnect(void);

		SOCKET* GetSocket(void);

		SOCKADDR_IN GetEndPoint(void);

		int GetEndPointLength(void);

		SocketRegistry* GetAcceptRegistry(void);

		SocketRegistry* GetConnectRegistry(void);

		SocketRegistry* GetSendRegistry(void);

		SocketRegistry* GetSendToRegistry(void);

		SocketRegistry* GetReceiveRegistry(void);

		SocketRegistry* GetReceiveFromRegistry(void);

		SocketRegistry* GetDisconnectRegistry(void);

	private:
		void SetSocket(const SOCKET& sock);
		void SetEndPoint(const SOCKADDR_IN& ep);
		void SetLen(const int& len);

		void HelperAcceptMethod(void);
		void HelperConnectMethod(void);
		void HelperSendMethod(char* data, int data_size);
		void HelperSendToMethod(char* data, int data_size);
		void HelperReceiveMethod(char* data, int data_size);
		void HelperReceiveFromMethod(char* data, int data_siz);
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
