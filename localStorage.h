#pragma once

#include "storageuser.h"

/*!
	@file
	@brief Файл класса хранилища пользователей
	@author Фонова Полина Викторовна
	@version 1.0
	@date январь 2023 года
*/

/*!
	@brief Класс хранилища пользователей
	@author Фонова Полина Викторовна
	@version 1.0
	@date январь 2023 года

	Представляет собой класс Singleton, который хранит в себе информацию о пользователях
*/
class UserStorage {
private:
	std::map< std::int64_t, User* > _users;

	UserStorage() {}
	UserStorage(const UserStorage& root) = delete;
	UserStorage& operator=(const UserStorage&) = delete;
public:
	/*!
		@brief Метод получения пользователя по его id
		@return указатель на экземпляр класса **User**

		Возвращает указатель на экземпляр класса **User**, 
		если пользователь с таким id уже существует,
		иначе создает нового пользователя и возвращает указатель на него
	*/
	User* operator [](std::int64_t id) {
		if (this->_users.find(id) == this->_users.end()) {
			User* newUser = new User(id);
			this->_users[id] = newUser;
		}
		return this->_users[id];
	}

	/*!
		@brief Метод получения экземпляра класса-одиночки **UserStorage**
		@return ссылку на экземпляр класса **UserStorage**

		Предоставляет глобальную точку доступа к единственному экземпляру хранилища
	*/
	static UserStorage& Instance()
	{
		static UserStorage theSingleInstance;
		return theSingleInstance;
	}
};


