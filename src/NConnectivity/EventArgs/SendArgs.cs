// © 2019 NIREX ALL RIGHTS RESERVED

using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class SendArgs : SocketArgs
    {
        public SendArgs(Socket sock, byte[] data, int size) : base(sock)
        {
            Data = data;
            SentSize = size;
        }

        public byte[] Data { get; protected set; }
        public int SentSize { get; protected set; }
    }
}
