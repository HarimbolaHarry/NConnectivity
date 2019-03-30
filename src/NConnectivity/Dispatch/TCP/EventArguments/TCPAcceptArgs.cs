using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    class TCPAcceptArgs : TCPArgs
    {
        public TCPAcceptArgs(Socket sock) : base(sock)
        {
            
        }
    }
}
