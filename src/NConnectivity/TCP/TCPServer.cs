// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace NConnectivity.TCP
{
    /// <summary>
    /// Mutli-client Async TCP Socket class Wrapper.
    /// </summary>
    public class TCPServer
    {
        byte[] rcvBuffer;
        byte[] sndBuffer;

        public int GeneralBufferSize { get; private set; }
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
        public TCPServer(string host, int port, int maxBufferSize = 1024)
        {
            rcvBuffer = new byte[maxBufferSize];
            sndBuffer = new byte[maxBufferSize];
            Connections = new List<Socket>();
            GeneralBufferSize = maxBufferSize;
            IpEndPoint = new IPEndPoint(IPAddress.Parse(host), port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Connection.Bind(IpEndPoint);
            Connection.Listen(int.MaxValue);
            Connection.BeginAccept(AcceptCallback, null);
        }

        /// <summary>
        /// Binds the main Socket to the LoopBack address and a given port and Begins listening for connections.
        /// </summary>
        /// <param name="port">Port of the EndPoint.</param>
        public TCPServer(int port, int maxBufferSize = 1024)
        {
            sndBuffer = new byte[maxBufferSize];
            sndBuffer = new byte[maxBufferSize];
            Connections = new List<Socket>();
            GeneralBufferSize = maxBufferSize;
            IpEndPoint = new IPEndPoint(IPAddress.Loopback, port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Connection.Bind(IpEndPoint);
            Connection.Listen(int.MaxValue);
            Connection.BeginAccept(AcceptCallback, null);
        }

        private void AcceptCallback(IAsyncResult ar)
        {
            Socket accepted;
            try
            {
                accepted = Connection.EndAccept(ar);
                Connections.Add(accepted);
                Connection.BeginAccept(AcceptCallback, null);
                SocketArgs args = new SocketArgs(accepted);
                Accept?.Invoke(this, args);
                accepted.BeginReceive(rcvBuffer, 0, GeneralBufferSize, SocketFlags.None, (ReceiveCallback), accepted);
            }
            catch
            {
                return;
            }
        }

        /// <summary>
        /// Send data to a specific host.
        /// </summary>
        /// <param name="sock">Connection Socket</param>
        /// <param name="buffer">Data to send</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginSend(Socket sock, byte[] buffer, SocketFlags flags = SocketFlags.None)
        {
            sndBuffer = buffer;
            sock.BeginSend(buffer, 0, buffer.Length, flags, new AsyncCallback(SendCallback), sock);
        }

        private void SendCallback(IAsyncResult ar)
        {
            Socket sock = (Socket)ar.AsyncState;
            sock.EndSend(ar);

            TransferArgs args = new TransferArgs(sock, sndBuffer, sndBuffer.Length);
            Send?.Invoke(this, args);
        }

        /// <summary>
        /// Broadcast a message to all the connected clients.
        /// </summary>
        /// <param name="buffer">Data to broadcast</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginBroadcast(byte[] buffer, SocketFlags flags = SocketFlags.None)
        {
            foreach (Socket socket in Connections)
            {
                socket.Send(buffer);
                TransferArgs args = new TransferArgs(socket, buffer, buffer.Length);
                Broadcast?.Invoke(this, args);
            }
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            Socket sock = (Socket)ar.AsyncState;
            
            int receivedBytes = 0;
            try
            {
                receivedBytes = sock.EndReceive(ar);
            }
            catch
            {
                sock.Close();
                Connections.Remove(sock);
                return;
            }

            TransferArgs args = new TransferArgs(sock, rcvBuffer, receivedBytes);
            Receive?.Invoke(this, args);
            sock.BeginReceive(rcvBuffer, 0, GeneralBufferSize, SocketFlags.None, (ReceiveCallback), sock);
        }

        /// <summary>
        /// Disconnect from a certain client reuse is false since the Socket object is removed from the list.
        /// </summary>
        /// <param name="sock">Socket to disconnect from</param>
        public void BeginDisconnect(Socket sock)
        {
            sock.Disconnect(false);
            SocketArgs args = new SocketArgs(sock);
            Connections.Remove(sock);
            Disconnect?.Invoke(this, args);
        }
    }
}
