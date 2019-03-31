using System;
using System.Net.Sockets;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    public class TCPArgs : EventArgs
    {
        public TCPArgs(Socket sock)
        {
            Connection = sock;
        }

        public Socket Connection { get; protected set; }
    }
}
