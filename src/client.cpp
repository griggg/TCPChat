#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>


class Client {
   public:
	Client() {}

	bool start() {
		client_fd = socket(AF_INET, SOCK_STREAM, 0);

		sockaddr_in server_addr{};
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(5637);
		if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
			close(client_fd);
			return 0;
		}

		if (connect(client_fd, (sockaddr *)&server_addr, sizeof(server_addr)) <
			0) {
			close(client_fd);
			return 0;
		}

		receiveThread = std::thread(&Client::receiveMessages, this);
		receiveThread.detach();
		running = true;
		 return 1;
	}
	bool read_n_bytes(char *buffer, size_t total_bytes) {
	size_t bytes_read = 0;
	while (bytes_read < total_bytes && running) {
		ssize_t result =
			read(client_fd, buffer + bytes_read, total_bytes - bytes_read);
		if (result <= 0) return false;
		bytes_read += result;
	}

	return true;
}
bool readMessage(std::string &out_message) {
	uint32_t net_length;
	read_n_bytes((char *)&net_length, sizeof(net_length));

	uint32_t msg_len = ntohl(net_length);
	std::vector<char> msg(msg_len + 1);
	read_n_bytes(msg.data(), msg_len);
	msg[msg_len] = '\0';
	out_message = std::string(msg.data());
	return true;
}
	bool send_message(const std::string &message) {
		uint32_t len = message.size();
		uint32_t net_len = htonl(len);

		if (send(client_fd, &net_len, sizeof(net_len), 0) != sizeof(net_len))
			return false;

		if (send(client_fd, message.c_str(), len, 0) != (ssize_t)len)
			return false;

		return true;
	}

	~Client() {
		if (client_fd != -1) close(client_fd);
		running = false;
	}

   private:
	int client_fd = -1;
	std::atomic<bool> running;
	std::thread receiveThread;

	void receiveMessages() {
		while (running) {
			std::string msg;
			readMessage(msg);
			std::cout << msg << std::endl;
		}
	}
};

int main() {
	Client client;
	client.start();
	while (1) {
		std::string command;
		std::getline(std::cin, command);
		
		if (command == "MKUSER") {
			std::string username;
			std::getline(std::cin, username);

			std::string password;
			std::getline(std::cin, password);
			client.send_message(command);
			client.send_message(username);
			client.send_message(password);
		} else if (command == "INUSER") {
		} else if (command == "MKROOM") {
			std::string name;
			std::getline(std::cin, name);

			client.send_message(command);
			client.send_message(name);
		} else if (command == "INROOM") {
			std::string id;
			std::getline(std::cin, id);

			client.send_message(command);
			client.send_message(id);

		} else if (command == "SNDMSG") {
			std::string roomId, msg;
			std::getline(std::cin, roomId);
			std::getline(std::cin, msg);

			client.send_message(command);
			client.send_message(roomId);
			client.send_message(msg);

		} else if (command == "ALROOM") {
			client.send_message(command);
		}
	}

	return 0;
}