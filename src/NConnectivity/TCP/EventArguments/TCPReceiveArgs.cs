using System.Net.Sockets;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    public class TCPReceiveArgs : TCPArgs
    {
        public TCPReceiveArgs(Socket sock, byte[] data, int size) : base(sock)
        {
            Data = data;
            ReceivedSize = size;
        }

        public byte[] Data { get; protected set; }

        public int ReceivedSize { get; protected set; }
    }
}
