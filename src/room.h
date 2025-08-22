#pragma once

#include <string>

#include "user.h"

class Room {
   public:
	std::string name;
	int id;	 ///< Уникальное id комнаты
	std::vector<std::shared_ptr<User>> members;

	Room(std::string name, int id) {
		this->name = name;
		this->id = id;
	}

	void addMember(std::shared_ptr<User> user) { members.push_back(user); }
};

/**
 * @brief Класс хранилище комнат
 *
 * Отвечает за корректное создание и получение комнат
 *
 */

class RoomDAO {
   public:
	RoomDAO() {}

	/**
	 * @brief Создаёт объект комнаты
	 *
	 * @param std::string name название комнаты (3-20 символов)
	 * @throw std::invalid_argument Если параметры невалидны
	 * @return User Созданный объект пользователя
	 *
	 * @code
	 * User user = User::create("john", "secret123");
	 * @endcode
	 */
	std::shared_ptr<Room> makeRoom(std::string name) {
		int id = -1;
		for (auto r : rooms) {
			id = std::max(r->id, id);
		}

		std::shared_ptr<Room> room = std::make_shared<Room>(name, id + 1);
		this->rooms.push_back(room);
		return room;
	}

	std::shared_ptr<Room> getById(int id) {
		for (auto r : rooms) {
			if (r->id == id) return r;
		}
		return nullptr;
	}

	const std::string roomList() {
		std::string roomNames;
		for (auto r : rooms) {
			roomNames += r->name;
			roomNames += '\n';
		}
		return roomNames;
	}

   private:
	std::vector<std::shared_ptr<Room>> rooms;
};
