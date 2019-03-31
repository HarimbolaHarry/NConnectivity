﻿using System;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace NConnectivity.TCP.Core
{
    /// <summary>
    /// Async TCP Client Socket class Wrapper.
    /// </summary>
    public class TCPClient
    {
        private byte[] rcvBuffer;
        private byte[] sendBuffer;

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
        public TCPClient(string host, int port)
        {
            IpEndPoint = new IPEndPoint(IPAddress.Parse(host), port);
            Connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        /// <summary>
        /// Begins the connection.
        /// </summary>
        public void BeginConnect()
        {
            Connection.BeginConnect(IpEndPoint, new AsyncCallback(ConnectCallback), Connection);
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            Connection.EndConnect(ar);

            ConnectArgs args = new ConnectArgs(Connection);
            Connect?.Invoke(this, args);
        }

        /// <summary>
        /// Send data to the server.
        /// </summary>
        /// <param name="buffer">Data to send</param>
        /// <param name="size">Size of the data</param>
        /// <param name="flags">Socket flags</param>
        public void BeginSend(byte[] buffer, int size, SocketFlags flags = SocketFlags.None)
        {
            rcvBuffer = buffer;
            Connection.BeginSend(buffer, 0, size, flags, new AsyncCallback(SendCallback), Connection);
        }

        private void SendCallback(IAsyncResult ar)
        {
            int sentBytes = Connection.EndSend(ar);

            SendArgs args = new SendArgs(Connection, sendBuffer, sentBytes);
            Send?.Invoke(this, args);
        }

        /// <summary>
        /// Receive data from the host.
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
            int receivedBytes = Connection.EndReceive(ar);

            ReceiveArgs args = new ReceiveArgs(Connection, rcvBuffer, receivedBytes);
            Receive?.Invoke(this, args);
        }

        /// <summary>
        /// Disconnect from a certain host false by default.
        /// </summary>
        public void BeginDisconnect()
        {
            Connection.BeginDisconnect(false, new AsyncCallback(DisconnectCallback), Connection);
        }

        private void DisconnectCallback(IAsyncResult ar)
        {
            Connection.EndDisconnect(ar);

            DisconnectArgs args = new DisconnectArgs(Connection);
            Disconnect?.Invoke(this, args);
        }
    }
}
