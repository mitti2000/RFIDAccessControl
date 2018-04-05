using System;
using accessControl.Core;

namespace accessControl.Console
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                System.Console.WriteLine("Pass the com port and the baudrate as arguments!");
                System.Console.ReadKey();
                return;
            }

            var port = args[0];
            var baudrate = Convert.ToInt32(args[1]);

            var serialCommunication = new SerialCommunication(port, baudrate);
            serialCommunication.StartListening(s => { System.Console.WriteLine("Data received: " + s); });
            ConsoleHelper.PrintIntroTitle();
            System.Console.ReadKey();

        }
    }
}
