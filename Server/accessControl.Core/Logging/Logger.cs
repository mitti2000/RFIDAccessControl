using System;

namespace accessControl.Core.Logging
{
    public class Logger
    {
        public static void Log(LogSeverity severity, string message)
        {
            Console.WriteLine($"{DateTime.Now} {severity.ToString().ToUpper()}:\t{message}");
        }
    }
}