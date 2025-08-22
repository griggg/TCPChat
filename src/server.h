#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "room.h"
#include "user.h"

class TCPServer {
   public:
	std::map<int, int> clients;
	std::map<std::shared_ptr<User>, int> usersToSockets;

	TCPServer(int port, RoomDAO &roomManager, UserDAO &userManager);

	bool start();

	void run();

	void sendMessage(int client_fd, const std::string &message);

	~TCPServer();

   private:
	RoomDAO roomManager;
	UserDAO userManager;

	int port;
	int server_fd;
	int cnt = 0;
	sockaddr_in address{};
	int addrlen = sizeof(address);

	std::mutex mtx;

	bool receive_message(int socket, std::string &out_message);

	bool read_n_bytes(int socket, char *buffer, size_t total_bytes);

	void handleClient(int clientSocket);
};
