// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace NConnectivity.UDP
{
    public class UDPBroadcaster
    {
        private IPEndPoint endPoint;
        private byte[] sendBuffer;

        public Socket Instance { get; private set; }

        public event ConnectionEventHandler Send;

        /// <summary>
        /// Initialize the Socket of the class.
        /// </summary>
        public UDPBroadcaster()
        {
            Instance = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        }

        /// <summary>
        /// Send some data to a specific EndPoint.
        /// </summary>
        /// <param name="address">The IP address of the host</param>
        /// <param name="port">The port number of the listener</param>
        /// <param name="buffer">The data to be sent</param>
        /// <param name="size">Size of the data to be sent</param>
        /// <param name="flags">Socket Flags</param>
        void BeginSendTo(string address, int port, byte[] buffer, int size, SocketFlags flags = SocketFlags.None)
        {
            endPoint = new IPEndPoint(IPAddress.Parse(address), port);
            sendBuffer = buffer;
            Instance.BeginSendTo(buffer, 0, size, flags, endPoint, (SendToCallback), Instance);
        }

        private void SendToCallback(IAsyncResult ar)
        {
            int sentSize = Instance.EndSendTo(ar);

            TransferArgs args = new TransferArgs(Instance, sendBuffer, sentSize);
            Send?.Invoke(this, args);
        }
    }
}
