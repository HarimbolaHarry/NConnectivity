using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class DisconnectArgs : SocketArgs
    {
        public DisconnectArgs(Socket sock) : base(sock)
        {
        }
    }
}
