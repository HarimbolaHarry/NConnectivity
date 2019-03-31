// © 2019 NIREX ALL RIGHTS RESERVED

using System;
using System.Text;
using NConnectivity.TCP;

namespace TCPTestServer
{
    class Server
    {
        static TCPServer server;

        static void Main(string[] args)
        {
            Console.Title = "Server";

            server = new TCPServer("127.0.0.1", 42422);
            server.Accept += Server_Accept;

            while (true) { }
        }

        private static void Server_Accept(object sender, NConnectivity.EventArgs.SocketArgs e)
        {
            Console.WriteLine("Client Connected!");
            byte[] x = Encoding.ASCII.GetBytes("Testing!\n");

            System.Threading.Thread.Sleep(1000);
            
            server.BeginSend(e.Connection, x);
            server.BeginSend(e.Connection, x);
            server.BeginSend(e.Connection, x);
        }
    }
}
