#pragma once

#include <deque>
#include "storagerecord.h"


/*!
	@file
	@brief Файл класса пользователей
	@author Фонова Полина Викторовна
	@version 1.0
	@date Январь 2023 года
*/


/*!
	@brief Класс пользователей
	@author Фонова Полина Викторовна
	@version 1.0
	@date Январь 2023 года

	Хранит информацию о пользователях и предоставляет методы для работы с ней.
	Также реализует хранение пользовательских запросов на распознавание текста на изображениях
	и позволяет ограничивать активность пользователей при превышении числа запросов на единицу
	времени.
*/
class User {
private:
	std::int64_t id;
	std::deque< Record* > _records;

public:
	const std::size_t MAX_COUNT_RECORDS = 10;			///< Максимальное количество записей в очереди
	const std::size_t MAX_COUNT_RECORDS_IN_PERIOD = 3;	///< Максимальное количество записей в течение периода времени
	const std::size_t PERIOD_ON_SECONDS = 60 * 3;		///< Период времени в секундах
	std::string language;						///< Язык распознавания текста

	/*!
		@brief Конструктор класса
		@param[in] id Идентификатор пользователя

		Создает пользователя с заданным идентификатором.
	*/
	User(std::int64_t id) {
		this->id = id;
		this->language = "en";
	}

	~User() {
		while (!this->_records.empty()) {
			delete this->_records.back();
			this->_records.pop_back();
		}
	}

	/*!
		@brief Метод добавления записи
		@param[in] text - распознанный текст на изображении
		@param[in] imageId - ID изображения
		@param[in] imagePath - URL изображения
		@param[in] dateMessage - время получения запроса, UTC+0

		Добавляет запись в историю запросов.
	*/
	void addRecord(std::string& text, std::string& imageId, std::string& imagePath, std::int32_t dateMessage) {
		this->_records.push_back(new Record(text, imageId, imagePath, dateMessage));
		if (this->_records.size() > this->MAX_COUNT_RECORDS) {
			delete this->_records.front();
			this->_records.pop_front();
		}
	}

	/*!
		@brief Метод получения истории запросов
		@return Список запросов пользователя

		Выводит только последние записи.
		Количество возвращаемых записей не превышает значения **MAX_COUNT_RECORDS**.
	*/
	std::vector< Record* > getRecords() {
		return std::vector< Record* >(this->_records.begin(), this->_records.end());
	}

	/*!
		@brief Количество запросов в истории пользователя
		@return Количество запросов в истории пользователя
		
		Учитываются только запросы, которые хранятся в истории на данный момент времени.
		Количество возвращаемых записей не превышает значения **MAX_COUNT_RECORDS**.
	*/
	int countRecords() {
		return int(this->_records.size());
	}
	
	/*!
		@brief Метод проверки превышения количества запросов на распознование текста
		@return true или false

		Проверяет, что за последние **PERIOD_ON_SECONDS** секунд было не менее **MAX_COUNT_RECORDS_IN_PERIOD** запросов.
		
		Возможные значения:
		- **true** - превышено количество запросов, необходимо подождать. Новые запросы до истечения времени
		будут проигнорированы;
		- **false** - количество запросов не превышено.
	*/
	bool isLimitRecords() {
		if (this->_records.size() < this->MAX_COUNT_RECORDS_IN_PERIOD) {
			return false;
		}

		std::int32_t dateNow = (std::int32_t)std::time(nullptr);
		auto currentrecord = this->_records.rbegin();
		for (unsigned int i = 1; i < this->MAX_COUNT_RECORDS_IN_PERIOD; i++) {
			currentrecord++;
		}
		if (dateNow - (*currentrecord)->getDateLocal() < (int32_t)this->PERIOD_ON_SECONDS) {
			return true;
		}
		return false;
	}
};
