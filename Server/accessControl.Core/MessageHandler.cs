using System;
using System.Collections.Generic;
using accessControl.Core.Auth;
using accessControl.Core.Data;
using accessControl.Core.Logging;

namespace accessControl.Core
{
    public class MessageHandler
    {
        private readonly Action<Message> _writeMessageDelegate;
        private readonly Dictionary<Function, Action<Message>> _handlers;
        private readonly UserAuthentication _authentication;

        public MessageHandler(Action<Message> writeMessageDelegate)
        {
            _writeMessageDelegate = writeMessageDelegate;
            _handlers = new Dictionary<Function, Action<Message>>();
            _handlers.Add(Function.id, HandleIdMessage);
            _handlers.Add(Function.invalid, HandleInvalidMessage);
            _handlers.Add(Function.pin, HandlePinMessage);

            _authentication = new UserAuthentication();
        }

        public void Handle(Message message)
        {
            if (!_handlers.ContainsKey(message.Function))
                throw new InvalidOperationException($"No handler for message type {message.Function} declared!");

            _handlers[message.Function].Invoke(message);
        }

        private void HandleIdMessage(Message message)
        {
            var userKey = Convert.ToInt64(message.Data);
            
            var user = UserRepository.Instance.Get(userKey);
            var userName = user != null ? user.Name : "fail";

            var returnMessage = new Message(Function.name, userName);

            _writeMessageDelegate.Invoke(returnMessage);
            Logger.Log(LogSeverity.Info, $"Return name {userName} for user with id {userKey}");
        }

        private void HandleInvalidMessage(Message message)
        {
            Logger.Log(LogSeverity.Error, "Error parsing message: " + message.Data);
        }

        private void HandlePinMessage(Message message)
        {
            var dataParts = message.Data.Split('#');
            var id = Convert.ToInt64(dataParts[0]);
            var password = dataParts[1].Substring(0, 4);

            var granted = _authentication.IsGranted(id, password);
            var responseMessage = new Message(Function.access, granted.ToString().ToLower());

            _writeMessageDelegate.Invoke(responseMessage);

            var grantLogMessage = granted ? "Access granted " : "Access denied";
            Logger.Log(LogSeverity.Info, $"{grantLogMessage} for user with id: {id}");
        }
    }
}