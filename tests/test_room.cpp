#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "room.h" 
#include "user.h"  


TEST(RoomTest, CreateRoom) {
    Room room("TestRoom", 1);
    EXPECT_EQ(room.name, "TestRoom");
    EXPECT_EQ(room.id, 1);
    EXPECT_TRUE(room.members.empty());
}

TEST(RoomTest, AddMember) {
    Room room("TestRoom", 1);
    auto user = std::make_shared<User>("Alice", "password");
    room.addMember(user);
    ASSERT_EQ(room.members.size(), 1);
    EXPECT_EQ(room.members[0]->username, "Alice");
}

TEST(RoomDAOTest, MakeRoomAssignsUniqueIds) {
    RoomDAO dao;
    auto room1 = dao.makeRoom("Room1");
    auto room2 = dao.makeRoom("Room2");

    EXPECT_EQ(room1->id + 1, room2->id);
    EXPECT_EQ(room1->name, "Room1");
    EXPECT_EQ(room2->name, "Room2");
}

TEST(RoomDAOTest, GetByIdReturnsCorrectRoom) {
    RoomDAO dao;
    auto room1 = dao.makeRoom("Room1");
    auto room2 = dao.makeRoom("Room2");

    auto found = dao.getById(room1->id);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->name, "Room1");

    EXPECT_EQ(dao.getById(9999), nullptr);  // Несуществующий id
}

TEST(RoomDAOTest, RoomListReturnsCorrectString) {
    RoomDAO dao;
    dao.makeRoom("Room1");
    dao.makeRoom("Room2");

    std::string list = dao.roomList();
    EXPECT_NE(list.find("Room1"), std::string::npos);
    EXPECT_NE(list.find("Room2"), std::string::npos);
}
