using System.Net.Sockets;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    public class TCPConnectArgs : TCPArgs
    {
        public TCPConnectArgs(Socket sock) : base(sock)
        {
        }
    }
}
