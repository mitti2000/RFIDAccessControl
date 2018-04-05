using System;

namespace accessControl.Core
{
    public class Message
    {
        public Message(Function function, string data)
        {
            Function = function;
            Data = data;
        }

        public Function Function { get; }

        public string Data { get; }

        public string Serialize()
        {
            return Function + "_" + Data;
        }

        public static Message Parse(string rawData)
        {
            Message message;

            try
            {
                var messageParts = rawData.Split('_');

                message = new Message
                (
                    (Function)Enum.Parse(typeof(Function), messageParts[0]),
                    messageParts[1]
                );
            }
            catch (Exception)
            {
                return new Message(Function.invalid, rawData);
            }

            return message;
        }
    }
}