#include <map>
#include <memory>

#include "room.h"
#include "server.h"
#include "user.h"

class RoomNotifier {
   public:
	TCPServer *server;
	std::string message;

	RoomNotifier(TCPServer *server, std::string message) {
		this->server = server;
		this->message = message;
	}

	void notify(std::shared_ptr<Room> room) {
		std::vector<std::shared_ptr<User>> users = room->members;
		for (auto i : users) {
			int clientSocket = server->usersToSockets[i];
			if (server->clients[clientSocket]) {
				server->sendMessage(clientSocket, message);
			}
		}
	}
};