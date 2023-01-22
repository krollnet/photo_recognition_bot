#pragma once

/*!
	@file
	@brief Файл класса записей хранилища пользовательских запросов
	@author Фонова Полина Викторовна
	@version 1.0
	@date Январь 2023 года
*/


/*!
	@brief Класс записей хранилища пользовательских запросов
	@author Фонова Полина Викторовна
	@version 1.0
	@date Январь 2023 года
	
	Экземпляры класса хранят информации о запросах пользователя и инкапсулируют следующую информацию:
	- результаты распознавания текста на изображении
	- свойства изображения
	- временные метки
	
	Предоставляют методы для получения и установки значений полей записи.
*/
class Record {
private:
	std::string result;
	std::string imageId;
	std::string imagePath;
	std::int32_t dateMessage;
	std::int32_t dateLocal;

public:
	/*!
		@brief Конструктор по умолчанию
		@param[in] result - распознанный текст на изображении
		@param[in] imageId - ID изображения
		@param[in] imagePath - URL изображения
		@param[in] dateMessage - время получения запроса, UTC+0
		
		Конструктор по умолчанию создает экземпляр класса Record с заданными значениями полей.
	*/
	Record(std::string result, std::string imageId, std::string imagePath, std::int32_t dateMessage) {
		this->result = result;
		this->imageId = imageId;
		this->imagePath = imagePath;
		this->dateMessage = dateMessage;
		this->dateLocal = (std::int32_t)std::time(nullptr);
	}
	
	~Record() = default;

	/*!
		@brief Метод получения распознанного текста на изображении
		@return Распознанный текст на изображении
	*/
	std::string getResult() {
		return this->result;
	}

	/*!
		@brief Метод получения ID изображения
		@return ID изображения (например, AgACAgIAAxkBAAICEWO0MhEFfkzwtDkUgt_Nj2kaP-I8AAKXxDEb75GgSXn7rZNVv-lKAQADAgADeQADLQQ)
	*/
	std::string getImageId() {
		return this->imageId;
	}

	/*!
		@brief Метод получения URL изображения
		@return URL изображения (например, https://api.telegram.org/file/bot%3Ctoken%3E/photos/file_1.jpg)
	*/
	std::string getImagePath() {
		return this->imagePath;
	}

	/*!
		@brief Метод получения времени получения запроса, UTC+0
		@return Время получения запроса, UTC+0
	*/
	std::int32_t getDateMessage() {
		return this->dateMessage;
	}

	/*!
		@brief Метод получения локального времени получения запроса
		@return Время получения запроса, локальное время
	*/
	std::int32_t getDateLocal() {
		return this->dateLocal;
	}
};
