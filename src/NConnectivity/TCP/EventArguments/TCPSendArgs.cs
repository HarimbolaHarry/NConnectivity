using System.Net.Sockets;

namespace NConnectivity.Dispatch.TCP.EventArguments
{
    public class TCPSendArgs : TCPArgs
    {
        public TCPSendArgs(Socket sock, byte[] data, int size) : base(sock)
        {
            Data = data;
            SentSize = size;
        }

        public byte[] Data { get; protected set; }
        public int SentSize { get; protected set; }
    }
}
