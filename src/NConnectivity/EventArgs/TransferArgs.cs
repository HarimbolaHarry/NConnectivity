// © 2019 NIREX ALL RIGHTS RESERVED

using System.Net.Sockets;

namespace NConnectivity.EventArgs
{
    public class TransferArgs : SocketArgs
    {
        public TransferArgs(Socket sock, byte[] data, int size) : base(sock)
        {
            Data = data;
            Size = size;
        }

        public byte[] Data { get; protected set; }

        public int Size { get; protected set; }

        /// <summary>
        /// Grab the byte[] from Data[0] to Data[Size]
        /// </summary>
        /// <returns>(byte[])Data[0] + ... + Data[Size]</returns>
        public byte[] Get()
        {
            byte[] bytes = new byte[Size];
            for (int i = 0; i < Size; i++)
            {
                bytes[i] = Data[i];
            }
            return bytes;
        }
    }
}
