using System;
using System.Net;
using System.Net.Sockets;
using NConnectivity.EventArgs;

namespace NConnectivity.UDP
{
    public class UDPReceiver
    {
        private byte[] rcvBuffer;
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

            Listener.BeginReceive(new AsyncCallback(ReceiveCallback), Listener);
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            IPEndPoint ipe = IpEndpoint;
            byte[] receivedData = Listener.EndReceive(ar, ref ipe);
            IpEndpoint = ipe;

            ReceiveArgs args = new ReceiveArgs(Listener.Client, receivedData, receivedData.Length);
            Receive?.Invoke(this, args);
        }
    }
}
