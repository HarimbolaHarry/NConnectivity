// © 2019 NIREX ALL RIGHTS RESERVED

using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class ConnectArgs : SocketArgs
    {
        public ConnectArgs(Socket sock) : base(sock)
        {
        }
    }
}
