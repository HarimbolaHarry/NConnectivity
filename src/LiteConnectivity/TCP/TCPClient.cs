// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Net;
using System.Net.Sockets;
using LiteConnectivity.EventArgs;

namespace LiteConnectivity.TCP
{
    /// <summary>
    /// Async TCP Client Socket class Wrapper.
    /// </summary>
    public class TCPClient
    {
        private byte[] rcvBuffer;
        private byte[] sndBuffer;

        public int GeneralBufferSize { get; private set; }
        public Socket Connection { get; private set; }
        public IPEndPoint IpEndPoint { get; private set; }

        public event ConnectionEventHandler Connect;
        public event ConnectionEventHandler Send;
        public event ConnectionEventHandler Receive;
        public event ConnectionEventHandler Disconnect;

        /// <summary>
        /// Initiates the required data for making a connection.
        /// </summary>
        /// <param name="host">The address of the server</param>
        /// <param name="port">The port of the server</param>
        public TCPClient(string host, int port, int bufferSize = 1024)
        {
            GeneralBufferSize = bufferSize;
            IpEndPoint = new IPEndPoint(IPAddress.Parse(host), port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Connection.BeginConnect(IpEndPoint, (ConnectCallback), Connection);

            rcvBuffer = new byte[1024];

            byte[] buffer = new byte[bufferSize];
            Connection.BeginReceive(buffer, 0, bufferSize, SocketFlags.None, ReceiveCallback, Connection);
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            Connection.EndConnect(ar);

            SocketArgs args = new SocketArgs(Connection);
            Connect?.Invoke(this, args);
        }

        /// <summary>
        /// Send data to the server.
        /// </summary>
        /// <param name="buffer">Data to send</param>
        /// <param name="flags">Socket flags</param>
        public void BeginSend(byte[] buffer, SocketFlags flags = SocketFlags.None)
        {
            sndBuffer = buffer;
            Connection.BeginSend(buffer, 0, buffer.Length, flags, new AsyncCallback(SendCallback), Connection);
        }

        private void SendCallback(IAsyncResult ar)
        {
            Connection.EndSend(ar);
            TransferArgs args = new TransferArgs(Connection, sndBuffer, sndBuffer.Length);
            Send?.Invoke(this, args);
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            int receivedBytes = Connection.EndReceive(ar);
            if (receivedBytes > 0)
            {
                Connection.BeginReceive(rcvBuffer, 0, GeneralBufferSize, SocketFlags.None, (ReceiveCallback), Connection);
                
                TransferArgs args = new TransferArgs(Connection, rcvBuffer, receivedBytes);
                Receive?.Invoke(this, args);
            }
        }

        /// <summary>
        /// Disconnect from a certain host false by default.
        /// </summary>
        public void BeginDisconnect()
        {
            Connection.BeginDisconnect(false, (DisconnectCallback), Connection);
        }

        private void DisconnectCallback(IAsyncResult ar)
        {
            Connection.EndDisconnect(ar);

            SocketArgs args = new SocketArgs(Connection);
            Disconnect?.Invoke(this, args);
        }
    }
}
