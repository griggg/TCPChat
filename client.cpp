#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <cstring>

class Client {
public:
    Client() {
        
    }

    bool start() {
        client_fd = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5578);
        if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
            close(client_fd);
            return 0;
        }

        if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(client_fd);
            return 0;
        }
        return 1;
    }

    bool send_message(const std::string& message) {
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
    }
private:
    int client_fd = -1;
};

int main() {

    Client client;
    client.start();
    while (1) {
        std::string msg;
        std::getline(std::cin, msg);
        client.send_message(msg);
    }

    
    return 0;

}