// © 2019 NIREX ALL RIGHTS RESERVED

using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class ReceiveArgs : SocketArgs
    {
        public ReceiveArgs(Socket sock, byte[] data, int size) : base(sock)
        {
            Data = data;
            ReceivedSize = size;
        }

        public byte[] Data { get; protected set; }

        public int ReceivedSize { get; protected set; }

        public byte[] Get()
        {
            byte[] bytes = new byte[ReceivedSize];
            for (int i = 0; i < ReceivedSize; i++)
            {
                bytes[i] = Data[i];
            }
            return bytes;
        }
    }
}
