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
        private byte[] rcvBuffer;

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
        public TCPServer(string host, int port, int size)
        {
            GeneralBufferSize = size;
            rcvBuffer = new byte[size];
            Connections = new List<Socket>();
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
        public TCPServer(int port, int size)
        {
            GeneralBufferSize = size;
            rcvBuffer = new byte[size];
            Connections = new List<Socket>();
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
            }
            catch
            {
                return;
            }

            Connections.Add(accepted);

            AcceptArgs args = new AcceptArgs(accepted);
            Accept?.Invoke(this, args);

            Connection.BeginAccept(AcceptCallback, null);
            accepted.BeginReceive(rcvBuffer, 0, GeneralBufferSize, SocketFlags.None, (ReceiveCallback), accepted);
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
            sock.Send(buffer);
            SendArgs args = new SendArgs(sock, buffer, buffer.Length);
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
                SendArgs args = new SendArgs(socket, buffer, buffer.Length);
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

            ReceiveArgs args = new ReceiveArgs(sock, rcvBuffer, receivedBytes);
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
            DisconnectArgs args = new DisconnectArgs(sock);
            Connections.Remove(sock);
            Disconnect?.Invoke(this, args);
        }
    }
}
