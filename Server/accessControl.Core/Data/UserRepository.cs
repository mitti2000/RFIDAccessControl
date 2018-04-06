using accessControl.Core.Auth;
using accessControl.Core.Logging;
using SQLite.Net;
using SQLite.Net.Platform.Generic;

namespace accessControl.Core.Data
{
    public class UserRepository
    {
        private readonly SQLiteConnection _db;

        private UserRepository()
        {
            var dbFilePath = @"D:\Projects\RFIDAccessControl\Server\accessControl.Core\Data\data.db";
            _db = new SQLiteConnection(new SQLitePlatformGeneric(), dbFilePath);
        }

        private static UserRepository _instance;

        public static UserRepository Instance => _instance ?? (_instance = new UserRepository());

        public User Get(long id)
        {
            User persistentUser;
            try
            {
                persistentUser = _db.Get<User>(id);
            }
            catch (System.Exception)
            {
                Logger.Log(LogSeverity.Warning, $"User with id '{id}' does not exist!");
                return null;
            }

            return persistentUser;
        }
    }
}