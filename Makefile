BUILD_DIR=build
SRC_DIR=src

.PHONY: client server all clean 

client: $(BUILD_DIR)/client.o
	g++ $< -o $(BUILD_DIR)/$@

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp
	g++ -c $< -o $@


server: $(BUILD_DIR)/server.o
	g++ $< -o $(BUILD_DIR)/$@ 

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.cpp
	g++ -c $< -o $@

all: client server
clean:
	rm -r build/*