#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

class TCPServer {
public:
    TCPServer(int port) {
        this->port = port;
        server_fd = -1;
    }

    bool start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            return 0;
        }
        
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_family = AF_INET;
        address.sin_port = htons(this->port);

        if (bind(server_fd, (sockaddr*)(&address), sizeof(address)) == -1) {
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

    void run() {
       while (1) {
            int client_fd = accept(server_fd, (sockaddr*)&address,  (socklen_t*)&addrlen);
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

    bool read_n_bytes(int socket, char* buffer, size_t total_bytes) {
        size_t bytes_read = 0;
        while (bytes_read < total_bytes) {
            ssize_t result = read(socket, buffer + bytes_read, total_bytes - bytes_read);
            if (result <= 0) {
                // Ошибка или клиент отключился
                return false;
            }
            bytes_read += result;
        }
        return true;
    }

    bool receive_message(int socket, std::string& out_message) {
        uint32_t net_length;
        if (!read_n_bytes(socket, (char*)&net_length, sizeof(net_length)))
            return false;

        uint32_t msg_length = ntohl(net_length); 
        if (msg_length == 0) return false;      

        char* buffer = new char[msg_length + 1];  
        if (!read_n_bytes(socket, buffer, msg_length)) {
            delete[] buffer;
            return false;
        }

        buffer[msg_length] = '\0'; 
        out_message = std::string(buffer);
        delete[] buffer;
        return true;
    }

    ~TCPServer() {
        if (server_fd != -1) close(server_fd);
    }
private:

    void sendMessage(int client_fd, std::string message) {
        uint32_t len = sizeof(message);
        uint32_t net_len = htonl(len);
        send(client_fd, &net_len, sizeof(net_len), 0);
        send(client_fd, message.c_str(),len, 0);
    }

    void notyfyClients(std::string msg, int sender) {
        std::unique_lock<std::mutex> lock(mtx);
        for (const auto &[client_fd, active] : clients) {
            if (active == 1 && client_fd != sender) {
                sendMessage(client_fd, msg);
            }
        }
    }

    void handleClient(int socket) {

        while (true) {
            std::string message;
            if (!receive_message(socket, message)) {
                std::cout << "Клиент отключился или ошибка\n";
                break;
            }

            std::cout << "Получено сообщение: " << message << std::endl;
            notyfyClients(message, socket);
        }
        close(socket);

        std::unique_lock<std::mutex> lock(mtx);
        clients[socket] = 0;
    }

    int port;
    int server_fd;
    int cnt = 0;
    sockaddr_in address{};
    int addrlen = sizeof(address);

    std::map<int, int> clients;

    std::mutex mtx;
};

int main() {
    
    
    TCPServer server(5582);

    server.start();
    server.run();
   
  
   

}
