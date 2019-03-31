// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Text;
using NConnectivity.TCP;

namespace TCPTestClient
{
    class Client
    {
        static TCPClient client;

        static void Main(string[] args)
        {
            Console.Title = "Client";

            client = new TCPClient("127.0.0.1", 42422, 1024);
            client.Receive += Client_Receive;
            client.Connect += Client_Connect;
            while (true) { }
        }

        private static void Client_Connect(object sender, NConnectivity.EventArgs.SocketArgs e)
        {
            Console.WriteLine("Connected!");
        }

        private static void Client_Receive(object sender, NConnectivity.EventArgs.SocketArgs e)
        {
            NConnectivity.EventArgs.ReceiveArgs args = (NConnectivity.EventArgs.ReceiveArgs)e;
            string x = Encoding.ASCII.GetString(args.Get());
            Console.Write(x);
        }
    }
}
