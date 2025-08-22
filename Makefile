BUILD_DIR=build
SRC_DIR=src

CFLAGS = -Wall -Wextra -Werror -std=cpp17

.PHONY: client server all clean 



all: client server
	doxygen
	clang-format -i src/*.h src/*.cpp  

client: $(BUILD_DIR)/client.o
	g++ $< -o $(BUILD_DIR)/$@

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp
	g++ -c $< -o $@


SERVER_SRC = $(SRC_DIR)/server.cpp $(SRC_DIR)/room.cpp $(SRC_DIR)/notifier.cpp $(SRC_DIR)/user.cpp
SERVER_OBJ = $(BUILD_DIR)/server.o $(BUILD_DIR)/room.o $(BUILD_DIR)/notifier.o $(BUILD_DIR)/user.o

server: $(SERVER_OBJ)
	g++ $^ -o $(BUILD_DIR)/$@ 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c $< -o $@

clean:
	rm -r build/*