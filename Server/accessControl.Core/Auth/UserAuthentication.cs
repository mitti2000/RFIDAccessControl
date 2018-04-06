using accessControl.Core.Data;

namespace accessControl.Core.Auth
{
    public class UserAuthentication
    {
        public bool IsGranted(long id, string password)
        {
            var user = UserRepository.Instance.Get(id);

            return password == user.Password;
        }
    }
}