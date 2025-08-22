// #include "user.h"

// class User {
// public:
//     std::string username;
//     std::string password;

//     User(std::string username, std::string passowrd): username(username),
//     password(password) {
//     }

// private:
// };

// class UserDAO {
// public:
//     UserDAO() {

//     }

//     std::shared_ptr<User> makeUser(std::string name, std::string password) {
//         for (auto u : users) {
//             if (u->username == name) throw std::invalid_argument("username не
//             уникальный");
//         }
//         std::shared_ptr<User> user = std::make_shared<User>(name, password);
//         return user;
//     }

//     std::shared_ptr<User> getByUserName(std::string username) {
//         for (auto u : users) {
//             if (u->username == username) return u;
//         }
//         return nullptr;
//     }
// private:
//     std::vector<std::shared_ptr<User>> users;
// };