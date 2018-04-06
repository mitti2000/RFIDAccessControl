using System;
using accessControl.Core;
using accessControl.Core.Logging;

namespace accessControl.Console
{
    class Program
    {
        private static MessageHandler _handler;

        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Logger.Log(LogSeverity.Error, "Pass the com port and the baudrate as arguments!");
                System.Console.ReadKey();
                return;
            }
            
            var port = args[0];
            var baudRate = Convert.ToInt32(args[1]);
            ConsoleHelper.PrintIntroTitle();

            var serialCommunication = new SerialCommunication(port, baudRate);
            Logger.Log(LogSeverity.Info, $"Server listening on {port} using a baudrate of {baudRate}");

            _handler = new MessageHandler(serialCommunication.Send);

            serialCommunication.StartListening(s =>
            {
                var message = Message.Parse(s);
                _handler.Handle(message);
            });

            System.Console.ReadKey();
        }
    }
}
