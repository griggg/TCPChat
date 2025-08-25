cmake_minimum_required(VERSION 3.11)

project(TCPChat)

set(CMAKE_CXX_STANDARD 17)

add_executable(client src/client.cpp)


add_executable(server src/server.cpp src/room.cpp src/notifier.cpp)

target_include_directories(server PUBLIC headers)

# Тесты

enable_testing()

add_executable(test_room tests/test_room.cpp)
target_include_directories(test_room PUBLIC headers)
target_link_libraries(test_room gtest_main gtest pthread)
add_test(NAME RoomTests COMMAND test_room)

add_executable(test_user tests/test_user.cpp)
target_include_directories(test_user PUBLIC headers)
target_link_libraries(test_user gtest_main gtest pthread)
add_test(NAME UserTests COMMAND test_user)
