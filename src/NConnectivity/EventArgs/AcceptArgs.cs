using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace NConnectivity.EventArgs
{
    class AcceptArgs : SocketArgs
    {
        public AcceptArgs(Socket sock) : base(sock)
        {
            
        }
    }
}
