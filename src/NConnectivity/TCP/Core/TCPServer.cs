using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace NConnectivity.TCP.Core
{
    /// <summary>
    /// Mutli-client Async TCP Socket class Wrapper.
    /// </summary>
    public class TCPServer
    {
        private byte[] rcvBuffer;
        private byte[] sendBuffer;

        public Socket Connection { get; private set; }
        public IPEndPoint IpEndPoint { get; private set; }
        public List<Socket> Connections { get; private set; }
        
        public event ConnectionEventHandler Accept;
        public event ConnectionEventHandler Send;
        public event ConnectionEventHandler Receive;
        public event ConnectionEventHandler Broadcast;
        public event ConnectionEventHandler Disconnect;

        /// <summary>
        /// Binds the main Socket to the given EndPoint and Begins listening for connections.
        /// </summary>
        /// <param name="host">IP Address to bind to.</param>
        /// <param name="port">Port of the EndPoint.</param>
        public TCPServer(string host, int port)
        {
            IpEndPoint = new IPEndPoint(IPAddress.Parse(host), port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Connection.Bind(IpEndPoint);
            Connection.Listen(int.MaxValue);
        }

        /// <summary>
        /// Binds the main Socket to the LoopBack address and a given port and Begins listening for connections.
        /// </summary>
        /// <param name="port">Port of the EndPoint.</param>
        public TCPServer(int port)
        {
            IpEndPoint = new IPEndPoint(IPAddress.Loopback, port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Connection.Bind(IpEndPoint);
            Connection.Listen(int.MaxValue);
        }

        /// <summary>
        /// Method to accept an incomming connection.
        /// </summary>
        public void BeginAccept()
        {
            Connection.BeginAccept(new AsyncCallback(AcceptCallback), Connection);
        }

        private void AcceptCallback(IAsyncResult ar)
        {
            Socket accepted = Connection.EndAccept(ar);
            Connections.Add(accepted);

            AcceptArgs args = new AcceptArgs(accepted);
            Accept?.Invoke(this, args);
        }

        /// <summary>
        /// Send data to a specific host.
        /// </summary>
        /// <param name="sock">Connection Socket</param>
        /// <param name="buffer">Data to send</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginSend(Socket sock, byte[] buffer, int size, SocketFlags flags = SocketFlags.None)
        {
            sendBuffer = buffer;
            sock.BeginSend(buffer, 0, size, flags, new AsyncCallback(SendCallback), sock);
        }

        private void SendCallback(IAsyncResult ar)
        {
            Socket sock = (Socket)ar.AsyncState as Socket;
            int sentBytes = sock.EndSend(ar);

            SendArgs args = new SendArgs(sock, sendBuffer, sentBytes);
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
            int sentBytes = Connection.EndSend(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            BroadcastArgs args = new BroadcastArgs(sock, sendBuffer, sentBytes);
            Send?.Invoke(this, args);
        }

        /// <summary>
        /// Receive data from any connection.
        /// </summary>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        /// <returns></returns>
        public byte[] BeginReceive(int size, SocketFlags flags = SocketFlags.None)
        {
            byte[] buffer = new byte[2048];
            Connection.BeginReceive(buffer, 0, size, flags, new AsyncCallback(ReceiveCallback), Connection);
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
            Socket sock = (Socket)ar.AsyncState as Socket;
            int receivedBytes = sock.EndReceive(ar);

            ReceiveArgs args = new ReceiveArgs(sock, rcvBuffer, receivedBytes);
            Receive?.Invoke(this, args);
        }

        /// <summary>
        /// Disconnect from a certain client reuse is false since the Socket object is removed from the list.
        /// </summary>
        /// <param name="sock">Socket to disconnect from</param>
        public void BeginDisconnect(Socket sock)
        {
            sock.BeginDisconnect(false, new AsyncCallback(DisconnectCallback), sock);
            Connections.Remove(sock);
        }

        private void DisconnectCallback(IAsyncResult ar)
        {
            Connection.EndDisconnect(ar);
            Socket sock = (Socket)ar.AsyncState as Socket;

            DisconnectArgs args = new DisconnectArgs(sock);
            Disconnect?.Invoke(this, args);
        }
    }
}
