using System.Collections.Generic;
using System.Linq;
using accessControl.Core.Logging;

namespace accessControl.Core.Auth
{
    public class UserRepository
    {
        private readonly IDictionary<long, User> _users;

        public UserRepository()
        {
            _users = new Dictionary<long, User>();
        }

        public void Add(User user)
        {
            if (_users.ContainsKey(user.Id))
                Logger.Log(LogSeverity.Error, $"User with id '{user.Id}' already exists!");
            else
                _users.Add(user.Id, user);
        }

        public User Get(long id)
        {
            if (_users.ContainsKey(id))
                Logger.Log(LogSeverity.Error, $"User with id '{id}' does not exist!");

            return _users.First(u => u.Key == id).Value;
        }

        public bool IsGranted(long id, string password)
        {
            var user = Get(id);

            return password == user.Password;
        }
    }
}