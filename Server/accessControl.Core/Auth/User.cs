using SQLite.Net.Attributes;

namespace accessControl.Core.Auth
{
    public class User
    {
        [PrimaryKey]
        public long Id { get; set; }

        public string Name { get; set; }

        public string Password { get; set; }

        public int Granted { get; set; }

        public int Denied { get; set; }
    }
}