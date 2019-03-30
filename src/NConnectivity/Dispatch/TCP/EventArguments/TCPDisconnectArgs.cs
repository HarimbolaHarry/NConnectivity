using System.Net.Sockets;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    public class TCPDisconnectArgs : TCPArgs
    {
        public TCPDisconnectArgs(Socket sock) : base(sock)
        {
        }
    }
}
