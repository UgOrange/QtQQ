class Server{
    private:
        int listen_sock;
        sockaddr_in local_addr;
        static int count;


    public:
        Server();
        void handleLogin();
        void hanldeUser();
        void regUser();
        void userLogin();
        void updateProfile();
        void forgetPassword();
        void getUserInfo();
        void getUserFriend();
        void getUserGroup();
        void addFriend();
        void delFriend();
        void handFriendRequest();
        void createGroup();
        void handGroupRequest();
        void sendMessage();
        void sendFile();
        void friendRequest();
        void groupRequest();
        void message();
        void file();

}
class fuction{
    public:

}