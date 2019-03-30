using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using NConnectivity.Dispatch.TCP.EventArguments;

public delegate void TCPServerEventHandler(object sender, TCPArgs e);

namespace NConnectivity.Dispatch.TCP.Core
{
    public class TCPServer
    {
        private byte[] rcvBuffer;
        private byte[] sendBuffer;

        public Socket Instance { get; private set; }
        public IPEndPoint IpEndPoint { get; private set; }
        public List<Socket> Connections { get; private set; }
        
        public event TCPServerEventHandler Accept;
        public event TCPServerEventHandler Send;
        public event TCPServerEventHandler Receive;
        public event TCPServerEventHandler Broadcast;
        public event TCPServerEventHandler Disconnect;

        /// <summary>
        /// Binds the main Socket to the given EndPoint and Begins listening for connections.
        /// </summary>
        /// <param name="host">IP Address to bind to.</param>
        /// <param name="port">Port of the EndPoint.</param>
        public TCPServer(string host, int port)
        {
            IpEndPoint = new IPEndPoint(IPAddress.Parse(host), port);
            Instance = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Instance.Bind(IpEndPoint);
            Instance.Listen(int.MaxValue);
        }

        /// <summary>
        /// Method to accept an incomming connection.
        /// </summary>
        public void BeginAccept()
        {
            Instance.BeginAccept(new AsyncCallback(AcceptCallback), Instance);
        }

        private void AcceptCallback(IAsyncResult ar)
        {
            Socket accepted = Instance.EndAccept(ar);
            Connections.Add(accepted);

            TCPAcceptArgs args = new TCPAcceptArgs(accepted);
            Accept?.Invoke(this, args);
        }

        /// <summary>
        /// Send a data to a specific host.
        /// </summary>
        /// <param name="sock">Connection Socket</param>
        /// <param name="buffer">Data to send</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginSend(Socket sock, byte[] buffer, int size, SocketFlags flags = SocketFlags.None)
        {
            sock.BeginSend(buffer, 0, size, flags, new AsyncCallback(SendCallback), sock);
            rcvBuffer = buffer;
        }

        private void SendCallback(IAsyncResult ar)
        {
            int sentBytes = Instance.EndSend(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            TCPSendArgs args = new TCPSendArgs(sock, sendBuffer, sentBytes);
            Send?.Invoke(this, args);
        }

        /// <summary>
        /// Broadcast a message to all the connected clients.
        /// </summary>
        /// <param name="buffer">Data to broadcast</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginBroadcast(byte[] buffer, int size, SocketFlags flags = SocketFlags.None)
        {
            foreach (Socket socket in Connections)
            {
                socket.BeginSend(buffer, 0, size, flags, new AsyncCallback(BroadcastCallback), socket);
            }
        }

        private void BroadcastCallback(IAsyncResult ar)
        {
            int sentBytes = Instance.EndSend(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            TCPBroadcastArgs args = new TCPBroadcastArgs(sock, sendBuffer, sentBytes);
            Send?.Invoke(this, args);
        }

        /// <summary>
        /// Receive data from any connection
        /// </summary>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        /// <returns></returns>
        public byte[] BeginReceive(int size, SocketFlags flags = SocketFlags.None)
        {
            byte[] buffer = new byte[2048];
            Instance.BeginReceive(buffer, 0, size, flags, new AsyncCallback(ReceiveCallback), Instance);
            rcvBuffer = buffer;

            byte[] properized_buffer = new byte[size];

            for (int i = 0; i < size; i++)
            {
                properized_buffer[i] = buffer[i];
            }

            return properized_buffer;
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            int receivedBytes = Instance.EndReceive(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            TCPReceiveArgs args = new TCPReceiveArgs(sock, rcvBuffer, receivedBytes);
            Receive?.Invoke(this, args);
        }

        /// <summary>
        /// Disconnect from a certain Socket
        /// </summary>
        /// <param name="sock">Socket to disconnect from</param>
        public void BeginDisconnect(Socket sock)
        {
            sock.BeginDisconnect(true, new AsyncCallback(DisconnectCallback), sock);
        }

        private void DisconnectCallback(IAsyncResult ar)
        {
            Instance.EndDisconnect(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            TCPDisconnectArgs args = new TCPDisconnectArgs(sock);
            Disconnect?.Invoke(this, args);
        }
    }
}
