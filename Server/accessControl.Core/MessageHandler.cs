using System;
using System.Collections.Generic;
using accessControl.Core.Auth;
using accessControl.Core.Logging;

namespace accessControl.Core
{
    public class MessageHandler
    {
        private readonly Action<Message> _writeMessageDelegate;
        private readonly Dictionary<Function, Action<Message>> _handlers;
        private readonly UserRepository _userRepository;

        public MessageHandler(Action<Message> writeMessageDelegate)
        {
            _writeMessageDelegate = writeMessageDelegate;
            _handlers = new Dictionary<Function, Action<Message>>();
            _handlers.Add(Function.id, HandleIdMessage);
            _handlers.Add(Function.invalid, HandleInvalidMessage);
            _handlers.Add(Function.access, HandlePinMessage);

            _userRepository = new UserRepository();
            _userRepository.Add(new User(5406, "Thomas", "1234"));
            _userRepository.Add(new User(13699, "Dominic", "1111"));
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
            
            var user = _userRepository.Get(userKey);
            var userName = user != null ? user.Name : "fail";

            var returnMessage = new Message(Function.name, userName);

            _writeMessageDelegate.Invoke(returnMessage);
        }

        private void HandleInvalidMessage(Message message)
        {
            Logger.Log(LogSeverity.Error, "Error parsing message: " + message.Data);
        }

        private void HandlePinMessage(Message message)
        {
            var dataParts = message.Data.Split('#');
            var id = Convert.ToInt64(dataParts[0]);
            var password = dataParts[1];

            var granted = _userRepository.IsGranted(id, password);
            var responseMessage = new Message(Function.access, granted.ToString().ToLower());

            _writeMessageDelegate.Invoke(responseMessage);
        }
    }
}