// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace LiteConnectivity.UDP
{
    public class UDPReceiver
    {
        public UdpClient Listener { get; private set; }
        public IPEndPoint IpEndpoint { get; private set; }

        public event ConnectionEventHandler Receive;

        /// <summary>
        /// Begin receiving data on the Loopback ip with a specified port.
        /// </summary>
        /// <param name="port">The port to listen for data on.</param>
        void BeginReceive(int port)
        {
            IpEndpoint = new IPEndPoint(IPAddress.Loopback, port);

            Listener.BeginReceive((ReceiveCallback), Listener);
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            IPEndPoint ipe = IpEndpoint;
            byte[] receivedData = Listener.EndReceive(ar, ref ipe);
            IpEndpoint = ipe;

            TransferArgs args = new TransferArgs(Listener.Client, receivedData, receivedData.Length);
            Receive?.Invoke(this, args);
        }
    }
}
