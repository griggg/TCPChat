#include "server.h"

#include "notifier.h"

TCPServer::TCPServer(int port, RoomDAO &roomManager, UserDAO &userManager) {
	this->port = port;
	server_fd = -1;
	this->roomManager = roomManager;
	this->userManager = userManager;
}

bool TCPServer::start() {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		return 0;
	}

	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_family = AF_INET;
	address.sin_port = htons(this->port);

	if (bind(server_fd, (sockaddr *)(&address), sizeof(address)) == -1) {
		std::cout << "error1";
		close(server_fd);
		return 0;
	}

	if (listen(server_fd, 100) == -1) {
		std::cout << "error2";
		close(server_fd);
		return 0;
	}

	return 1;
}

void TCPServer::run() {
	while (1) {
		int client_fd =
			accept(server_fd, (sockaddr *)&address, (socklen_t *)&addrlen);
		if (client_fd < 0) {
			std::cout << "error3";
			close(server_fd);
			return;
		}
		std::thread t(&TCPServer::handleClient, this, client_fd);
		t.detach();
		std::unique_lock<std::mutex> lock(mtx);
		clients[client_fd] = 1;
	}
}

void TCPServer::sendMessage(int client_fd, const std::string &message) {
	uint32_t len = sizeof(message);
	uint32_t net_len = htonl(len);
	send(client_fd, &net_len, sizeof(net_len), 0);
	send(client_fd, message.c_str(), len, 0);
}

TCPServer::~TCPServer() {
	if (server_fd != -1) close(server_fd);
}

bool TCPServer::read_n_bytes(int socket, char *buffer, size_t total_bytes) {
	size_t bytes_read = 0;
	while (bytes_read < total_bytes) {
		ssize_t result =
			read(socket, buffer + bytes_read, total_bytes - bytes_read);
		if (result <= 0) {
			// Ошибка или клиент отключился
			return false;
		}
		bytes_read += result;
	}
	return true;
}

bool TCPServer::receive_message(int socket, std::string &out_message) {
	uint32_t net_length;
	if (!read_n_bytes(socket, (char *)&net_length, sizeof(net_length)))
		return false;

	uint32_t msg_length = ntohl(net_length);
	if (msg_length == 0) return false;

	char *buffer = new char[msg_length + 1];
	if (!read_n_bytes(socket, buffer, msg_length)) {
		delete[] buffer;
		return false;
	}

	buffer[msg_length] = '\0';
	out_message = std::string(buffer);
	delete[] buffer;
	return true;
}

void TCPServer::handleClient(int clientSocket) {
	// std::unique_lock<std::mutex> lk(mtx);
	while (true) {
		std::string command;
		if (!receive_message(clientSocket, command)) {
			std::cout << "Клиент отключился или ошибка\n";
			break;
		}
		if (command == "MKROOM") {
			std::cout << "MKROOM command-----" << std::endl;
			std::string roomName;
			receive_message(clientSocket, roomName);

			std::lock_guard<std::mutex> lk(mtx);
			std::shared_ptr<Room> room = roomManager.makeRoom(roomName);
			sendMessage(clientSocket, "Success MKROOM");

		} else if (command == "INROOM") {
		} else if (command == "MKUSER") {
			std::string username;
			receive_message(clientSocket, username);
			std::string password;
			receive_message(clientSocket, password);

			std::shared_ptr<User> user =
				userManager.makeUser(username, password);
			if (user == nullptr) continue;
			usersToSockets[user] = clientSocket;
			std::cout << "Создан юзер" << std::endl;

		} else if (command == "INUSER") {
			std::string username;
			receive_message(clientSocket, username);
			std::string password;
			receive_message(clientSocket, password);

			std::shared_ptr<User> user = userManager.getByUserName(username);
			if (user == nullptr) continue;
			if (!user->checkPassword(password)) continue;

			{
				std::lock_guard<std::mutex> lk(mtx);
				usersToSockets[user] = clientSocket;
			}

		} else if (command == "SNDMSG") {
			std::string roomId;
			receive_message(clientSocket, roomId);

			std::string msg;
			receive_message(clientSocket, msg);

			std::lock_guard<std::mutex> lk(mtx);
			std::shared_ptr<Room> room = roomManager.getById(std::stoi(roomId));
			RoomNotifier notifier(this, msg);
			notifier.notify(room);
		} else if (command == "ALROOM") {
			const std::string roomsList = roomManager.roomList();
			sendMessage(clientSocket, roomsList);
		} else {
			sendMessage(clientSocket, "Unkown command");
		}
	}
	close(clientSocket);

	std::unique_lock<std::mutex> lock(mtx);
	clients[clientSocket] = 0;
}

int main() {
	UserDAO userManager;
	RoomDAO roomManager;
	TCPServer server(5588, roomManager, userManager);
	server.start();

	server.run();
}
