#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <iostream>
#include "user.h"  


TEST(UserTest, CreateUser) {

    
    UserDAO userManager;
    userManager.makeUser("1", "1");

    // тест на создание юзеров с одинаковым именем
    EXPECT_THROW(userManager.makeUser("1", "1"),  std::invalid_argument);
    // проверка на корректное создание
    ASSERT_EQ(userManager.getUsers()[0]->password, "1");
}

