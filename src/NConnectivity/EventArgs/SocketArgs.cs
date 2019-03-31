using System;
using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class SocketArgs : EventArgs
    {
        public SocketArgs(Socket sock)
        {
            Connection = sock;
        }

        public Socket Connection { get; protected set; }
    }
}
