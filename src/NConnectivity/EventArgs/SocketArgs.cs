using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class SocketArgs : System.EventArgs
    {
        public SocketArgs(Socket sock)
        {
            Connection = sock;
        }

        public Socket Connection { get; protected set; }
    }
}
