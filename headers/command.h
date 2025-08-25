
enum class CommandType {
    MKROOM = 0,    // Создание комнаты
    MKUSER = 1,    // Создание пользователя
    INROOM = 2,    // Вход в комнату
    INUSER = 3,    // Вход пользователя
    SNDMSG = 4,    // Отправка сообщения
    ALROOM = 5,    // Список комнат
    UNKNOWN = 99   // Неизвестная команда
};

const char* commandTypeToString(CommandType type) {
    switch (type) {
        case CommandType::MKROOM: return "MKROOM";
        case CommandType::MKUSER: return "MKUSER";
        case CommandType::INROOM: return "INROOM";
        case CommandType::INUSER: return "INUSER";
        case CommandType::SNDMSG: return "SNDMSG";
        case CommandType::ALROOM: return "ALROOM";
        default: return "UNKNOWN";
    }
}

CommandType stringToCommandType(const std::string& str) {
    if (str == "MKROOM") return CommandType::MKROOM;
    if (str == "MKUSER") return CommandType::MKUSER;
    if (str == "INROOM") return CommandType::INROOM;
    if (str == "INUSER") return CommandType::INUSER;
    if (str == "SNDMSG") return CommandType::SNDMSG;
    if (str == "ALROOM") return CommandType::ALROOM;
    return CommandType::UNKNOWN;
}

// class ICommand {
// public:

//     virtual void execute() = 0;
// };

// class MakeRoomCommand : ICommand {
// private
// public:
//     MakeRoomCommand(std::string name) {

//     }
// };

// class CommandFactory {
// public:
//     CommandFactory(std::string command) {
//         if (command == "MKROOM") {

//         } else if (command == "RMROOM") {

//         }
//     }
// };