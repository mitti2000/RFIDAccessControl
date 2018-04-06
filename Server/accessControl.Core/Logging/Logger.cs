using System;

namespace accessControl.Core.Logging
{
    public class Logger
    {
        public static void Log(LogSeverity severity, string message)
        {
            Console.ForegroundColor = GetColor(severity);
            Console.WriteLine($"{DateTime.Now} {severity.ToString().ToUpper()}:\t{message}");
        }

        private static ConsoleColor GetColor(LogSeverity severity)
        {
            switch (severity)
            {
                case LogSeverity.Debug:
                    return ConsoleColor.Gray;
                case LogSeverity.Info:
                    return ConsoleColor.White;
                case LogSeverity.Warning:
                    return ConsoleColor.Yellow;
                case LogSeverity.Error:
                    return ConsoleColor.Red;
                default:
                    return ConsoleColor.White;
            }
        }
    }
}