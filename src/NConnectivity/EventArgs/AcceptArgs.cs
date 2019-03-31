// © 2019 NIREX ALL RIGHTS RESERVED

using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    class AcceptArgs : SocketArgs
    {
        public AcceptArgs(Socket sock) : base(sock)
        {
            
        }
    }
}
