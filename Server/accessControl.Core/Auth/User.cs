namespace accessControl.Core.Auth
{
    public class User
    {
        public User(long id, string name, string password)
        {
            Id = id;
            Name = name;
            Password = password;
        }

        public long Id { get; }

        public string Name { get; }

        public string Password { get; }
    }
}