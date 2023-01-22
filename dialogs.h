#pragma once

#include <fstream>


/*!
	@file
	@brief Файл логики диалогов
	@author Фонова Полина Викторовна
	@version 1.0
	@date Январь 2023 года
*/


nlohmann::json dialogsJson;                                        //!< JSON-объект, хранящий информацию о диалогах

const std::string filenameDialogs = "config/dialogs.json";         //!< Путь к JSON файлу с диалогами
const std::string baseLanguage = "en";                             //!< Язык по умолчанию для диалогов

const std::string GREETINGS = "greetings";                         //!< Ключ для приветствия
const std::string HELP = "help";                                   //!< Ключ для справки
const std::string INFO = "info";                                   //!< Ключ для информации о боте
const std::string HINT = "hint";                                   //!< Ключ для подсказки
const std::string SELECT_LANGUAGE = "selectLanguage";              //!< Ключ для выбора языка
const std::string LANGUAGES_BUTTONS = "languagesButtons";          //!< Ключ для кнопок языков
const std::string ERROR_BLOCK = "Error";                           //!< Ключ для словаря ошибок
const std::string ERROR_N0_PHOTO = "noPhoto";                      //!< Ключ для ошибки отсутствия фото
const std::string ERROR_TOO_MANY_PHOTOS = "tooManyPhotos";         //!< Ключ для ошибки превышения количества фотографий
const std::string ERROR_EMPTY_HISTORY = "emptyHistory";            //!< Ключ для ошибки пустой истории

/*!
	@brief Процедура инициализации диалогов
	
	Считывает диалоги из JSON файла **filenameDialogs** и сохраняет их в словарь.
*/
void initialDialogs() {
	std::ifstream f(filenameDialogs);
	dialogsJson = nlohmann::json::parse(f);
	f.close();
}

/*!
	@brief Функция получения диалога

	@param dialog Диалог (объект класса **nlohmann::json**, хранящий диалог на разных языках)
	@param language Язык диалога
	@return Строка с диалогом

	Возвращает диалог на языке **language**. Если диалога на этом языке нет, то возвращает диалог на языке **baseLanguage**.
	Если диалога на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string selectLanguage(const nlohmann::json dialog, const std::string& language) {
	try {
		if (dialog.find(language) == dialog.end()) {
			return dialog[baseLanguage];
		}
		return dialog[language];
	}
	catch (nlohmann::json::exception& ) {
		return "";
	}
}

/*!
	@brief Функция получения диалога

	@param language Язык диалога
	@param countKeys Количество ключей для доступа к диалогу в JSON-файле (по умолчанию 1)
	@param ... Ключи для доступа к диалогу в JSON-файле (строки **std::string**)
	@return Строка с диалогом

	Возвращает диалог на языке **language** по указанной последовательности ключей.
	Если диалога на этом языке нет, то возвращает диалог на языке **baseLanguage**.
	Если диалога на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string getDialog(const std::string& language, const size_t countKeys, ...) {
	try {
		va_list arguments;
		va_start(arguments, countKeys);
		nlohmann::json dialog = dialogsJson;
		for (size_t i = 0; i < countKeys; ++i) {
			std::string key = va_arg(arguments, std::string);
			dialog = dialog[key];
		}
		return selectLanguage(dialog, language);
	}
	catch (nlohmann::json::exception& ) {
		return "";
	}
}

/*!
	@brief Функция получения диалога приветствия
	@param language Язык диалога
	@return Строка приветствия

	Возвращает диалог приветствия на языке **language**.
	Если диалога приветствия на этом языке нет, то возвращает диалог приветствия на языке **baseLanguage**.
	Если диалога приветствия на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogGreeting(const std::string& language) {
	return getDialog(language, 1, GREETINGS);
}

/*!
	@brief Функция получения текста справки
	@param language Язык справки
	@return Текст справки

	Возвращает текст справки на языке **language**.
	Если текста справки на этом языке нет, то возвращает текст справки на языке **baseLanguage**.
	Если текста справки на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogHelp(const std::string& language) {
	return getDialog(language, 1, HELP);
}

/*!
	@brief Функция получения информации о боте
	@param language Язык информации о боте
	@return Текст информации о боте

	Возвращает текст информации о боте на языке **language**.
	Если текста информации о боте на этом языке нет, то возвращает текст информации о боте на языке **baseLanguage**.
	Если текста информации о боте на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogInfo(const std::string& language) {
	return getDialog(language, 1, INFO);
}

/*!
	@brief Функция получения текста подсказки
	@param language Язык подсказки
	@return Текст подсказки

	Возвращает текст подсказки на языке **language**.
	Если текста подсказки на этом языке нет, то возвращает текст подсказки на языке **baseLanguage**.
	Если текста подсказки на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogHint(const std::string& language) {
	return getDialog(language, 1, HINT);
}

/*!
	@brief Функция получения диалога смены языка
	@param language Текущий язык
	@return Текст диалога смены языка

	Возвращает диалог смены языка на языке **language**.
	Если диалога смены языка на этом языке нет, то возвращает диалог смены языка на языке **baseLanguage**.
	Если диалога смены языка на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogSelectLanguage(const std::string& language) {
	return getDialog(language, 1, SELECT_LANGUAGE);
}

/*!
	@brief Функция получения подписи кнопки смены языка
	@param language Язык кнопки
	@return Текст подписи кнопки

	Возвращает текст подписи кнопки на языке **language**.
	Если кнопки нет, то возвращает пустую строку.
*/
std::string dialogLanguagesButtons(const std::string& language) {
	return getDialog(language, 1, LANGUAGES_BUTTONS);
}

/*!
	@brief Функция получения текста ошибки отсутствия фото
	@param language Язык ошибки
	@return Текст ошибки

	Возвращает текст ошибки отсутствия фото на языке **language**.
	Если текста ошибки на этом языке нет, то возвращает текст ошибки на языке **baseLanguage**.
	Если текста ошибки на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogErrorNoPhoto(const std::string& language) {
	return getDialog(language, 2, ERROR_BLOCK, ERROR_N0_PHOTO);
}

/*!
	@brief Функция получения текста ошибки превышения количества фотографий
	@param language Язык ошибки
	@return Текст ошибки

	Возвращает текст ошибки отсутствия фото на языке **language**.
	Если текста ошибки на этом языке нет, то возвращает текст ошибки на языке **baseLanguage**.
	Если текста ошибки на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogErrorTooManyPhotos(const std::string& language) {
	return getDialog(language, 2, ERROR_BLOCK, ERROR_TOO_MANY_PHOTOS);
}

/*!
	@brief Функция получения текста ошибки отсутствия истории запросов
	@param language Язык ошибки
	@return Текст ошибки

	Возвращает текст ошибки отсутствия фото на языке **language**.
	Если текста ошибки на этом языке нет, то возвращает текст ошибки на языке **baseLanguage**.
	Если текста ошибки на языке **baseLanguage** нет, то возвращает пустую строку.
*/
std::string dialogErrorEmptyHistory(const std::string& language) {
	return getDialog(language, 2, ERROR_BLOCK, ERROR_EMPTY_HISTORY);
}
