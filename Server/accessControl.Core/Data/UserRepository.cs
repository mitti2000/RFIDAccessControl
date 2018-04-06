using System;
using System.Collections.Generic;
using System.Linq;
using accessControl.Core.Auth;
using accessControl.Core.Logging;

namespace accessControl.Core.Data
{
    public class UserRepository
    {
        private readonly IDictionary<long, User> _users;

        private UserRepository()
        {
            _users = new Dictionary<long, User>();
        }

        private static UserRepository _instance;

        public static UserRepository Instance => _instance ?? (_instance = new UserRepository());

        public void Add(User user)
        {
            if (_users.ContainsKey(user.Id))
                Logger.Log(LogSeverity.Error, $"User with id '{user.Id}' already exists!");
            else
                _users.Add(user.Id, user);
        }

        public void Update(User user)
        {
            throw new NotImplementedException();
        }

        public User Get(long id)
        {
            if (!_users.ContainsKey(id))
                Logger.Log(LogSeverity.Error, $"User with id '{id}' does not exist!");

            return _users.First(u => u.Key == id).Value;
        }
    }
}