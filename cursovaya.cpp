// cursovaya.cpp: определяет точку входа для приложения.
//
#pragma warning(disable :5045)

#include "cursovaya.h"
#include "dialogs.h"
#include "localStorage.h"

/*!
    @file
    @brief Файл класса пользователей
    @author Фонова Полина Викторовна
    @version 1.0
    @date Январь 2023 года
*/

/*!
	@brief Функция получения токена для Telegram API
	@return Строка с токеном
    
	Функция возвращает токен для Telegram API из файла *config/token.txt*.
*/
std::string getToken();

/*!
	@brief Функция распознавания текста на изображении по имени файла
	@param[in] filename Путь до изображения
	@return Строка с распознанным текстом
*/
std::string ocrImageFile(std::string& filename);

/*!
	@brief Функция распознавания текста на изображении по объекту изображения
	@param[in] imageData Объект изображения в виде байт-строки
	@return Строка с распознанным текстом
*/
std::string ocrImageData(std::string& imageData);

/*!
	@brief Процедура инициализации **tesseractApi**
*/
void initialTesseract();

/*!
	@brief Процедура высвобождения памяти, занятой **tesseractApi**
*/
void freeTesseract();

/*!
    @brief Процедура смены языка пользовательского интерфейса бота
	@param bot Ссылка на объект бота
	@param message Объект сообщения
*/
void changeLanguage(TgBot::Bot& bot, TgBot::Message::Ptr message);

/*!
	@brief Функция получения клавиатуры для выбора языка
	@return Объект клавиатуры
*/
TgBot::ReplyKeyboardMarkup::Ptr getReplyKeyboardMarkup();

/*!
    @param bot Ссылка на объект бота
    @brief Функция отправки сообщения пользователю
	@param chatId Идентификатор чата
	@param text Текст сообщения
	@param replyToMessageId Идентификатор сообщения, на которое отвечает бот (по умолчанию 0 - нет такого сообщения)
	@param keyboard Указатель на объект клавиатуры (nullptr по умолчанию)
*/
void sendMessage(
    const TgBot::Bot& bot,
    std::int64_t chatId,
    const std::string& text,
    std::int32_t replyToMessageId = 0,
    TgBot::ReplyKeyboardMarkup::Ptr keyboard = nullptr
);

tesseract::TessBaseAPI* tesseractApi = nullptr;                         //!< Объект **tesseractApi** для распознавания текста на изображении
TgBot::ReplyKeyboardMarkup::Ptr keyboard = nullptr;                     //!< Объект клавиатуры для выбора языка
std::shared_ptr<TgBot::ReplyKeyboardRemove> removeKeyboard = nullptr;   //!< Объект для удаления клавиатуры

/*!
	@brief Множество комманд бота
*/
std::set<std::string> commands = { "/start", "/help", "/info", "/lang", "/history"};

/*!
    @brief Список поддерживаемых языков интерфейса
*/
std::vector<std::string> languages = { "en", "ru" };

/*!
 * @brief Точка входа в приложение
 * @return 0 если приложение завершилось корректно, иное число в случае ошибки
*/
int main() {
    initialDialogs();
    initialTesseract();
    keyboard = getReplyKeyboardMarkup();
    TgBot::Bot bot(getToken());

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        std::string currentLanguage = UserStorage::Instance()[message->chat->id]->language;
	    sendMessage(bot, message->chat->id, dialogGreeting(currentLanguage));
        changeLanguage(bot, message);
    });
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
		std::string currentLanguage = UserStorage::Instance()[message->chat->id]->language;
        sendMessage(bot, message->chat->id, dialogHelp(currentLanguage));
    });
    bot.getEvents().onCommand("info", [&bot](TgBot::Message::Ptr message) {
        std::string currentLanguage = UserStorage::Instance()[message->chat->id]->language;
        sendMessage(bot, message->chat->id, dialogInfo(currentLanguage));
        sendMessage(bot, message->chat->id, dialogHint(currentLanguage));
    });
    bot.getEvents().onCommand("history", [&bot](TgBot::Message::Ptr message) {
        User* user = UserStorage::Instance()[message->chat->id];
        std::string currentLanguage = user->language;
        if (user->countRecords() == 0) {
            sendMessage(bot, message->chat->id, dialogErrorEmptyHistory(currentLanguage));
        }
        else {
            for (auto& record : user->getRecords()) {
                std::string text = "";
                date::sys_seconds tp{ std::chrono::seconds{record->getDateMessage()}};
                text += date::format("%Y-%m-%d %I:%M:%S %p", tp) + " GMT+0\n\n";
                text += record->getResult();
                sendMessage(bot, message->chat->id, text);
            }
        }
    });
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        clock_t tStart = clock();
        User* user = UserStorage::Instance()[message->chat->id];
        std::string currentLanguage = user->language;

		if (message->text.find("/lang", 0) == 0) {
            if (message->text.size() < 6) {
                changeLanguage(bot, message);
                return;
            }
            std::string newLanguage = message->text.substr(6, 2);
            if (std::find(languages.begin(), languages.end(), newLanguage) != languages.end()) {
				UserStorage::Instance()[message->chat->id]->language = newLanguage;
                sendMessage(bot, message->chat->id, dialogInfo(newLanguage));
                sendMessage(bot, message->chat->id, dialogHint(newLanguage));
            }
            else {
                changeLanguage(bot, message);
            }
			return;
		}
		for (auto& command : commands) {
			if (message->text.find(command) != std::string::npos) {
				return;
			}
		}
		if (message->photo.empty()) {
            sendMessage(bot, message->chat->id, dialogErrorNoPhoto(currentLanguage));
			return;
		}
        if (user->isLimitRecords()) {
            sendMessage(bot, message->chat->id, dialogErrorTooManyPhotos(currentLanguage));
            return;
        }
        
		std::string fileId = message->photo.back()->fileId;
		std::string filePath = bot.getApi().getFile(fileId)->filePath;   
		std::string imageData = bot.getApi().downloadFile(filePath);
		// std::ofstream file(filename, std::ios::binary); // output file stream - передача данных из нашего кода в файл
		// file << imageData;
		// file.close();

        //std::string text = ocrImageFile(filename);
        std::string text = ocrImageData(imageData);
        user->addRecord(text, fileId, filePath, message->date);
		sendMessage(bot, message->chat->id, text, message->messageId);
        sendMessage(bot, message->chat->id, dialogHint(currentLanguage));

        printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
        freeTesseract();
        delete keyboard.get();
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}

void sendMessage(
    const TgBot::Bot& bot,
    std::int64_t chatId,
    const std::string& text,
    std::int32_t replyToMessageId,
    TgBot::ReplyKeyboardMarkup::Ptr keyboard
) {
    try {
        try {
            bot.getApi().sendMessage(chatId, text, false, replyToMessageId, keyboard);
        }
        catch (TgBot::TgException& e) {
            if (replyToMessageId != 0) {
				bot.getApi().sendMessage(chatId, text, false, 0, keyboard);
			}
			else {
				throw e;
            }
        }
    }
	catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
	}
}

std::string getToken() {
    std::ifstream file("config/token.txt");
    std::string token;
    file >> token;
    file.close();
    return token;
}

std::string ocrImageFile(std::string& filename) {
    Pix* image = pixRead(filename.c_str());
    tesseractApi->SetImage(image);
    char *text = tesseractApi->GetUTF8Text();
    std::string result = text;
    delete[] text;
    pixDestroy(&image);
    return result;
}

std::string ocrImageData(std::string& imageData) {
	Pix* image = pixReadMem((const unsigned char*)imageData.c_str(), imageData.size());
	tesseractApi->SetImage(image);
	char* text = tesseractApi->GetUTF8Text();
	std::string result = text;
	delete[] text;
	pixDestroy(&image);
	return result;
}

void initialTesseract() {
    tesseractApi = new tesseract::TessBaseAPI();
    if (tesseractApi->Init(NULL, "eng+rus")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(2);
    }  
}

void freeTesseract() {
    tesseractApi->End();
    delete tesseractApi;
}

TgBot::ReplyKeyboardMarkup::Ptr getReplyKeyboardMarkup() {
    TgBot::ReplyKeyboardMarkup::Ptr keyboardMarkup(new TgBot::ReplyKeyboardMarkup);
    keyboardMarkup->resizeKeyboard = true;
	for (size_t i = 0; i < languages.size(); i++) {
		if (i % 2 == 0) {
			keyboardMarkup->keyboard.push_back(std::vector<TgBot::KeyboardButton::Ptr>());
		}
		TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
		button->text = "/lang " + languages[i] + "\n\n" + dialogLanguagesButtons(languages[i]);
		keyboardMarkup->keyboard.back().push_back(button);
	}
	keyboardMarkup->oneTimeKeyboard = true;
    return keyboardMarkup;
}

void changeLanguage(TgBot::Bot& bot, TgBot::Message::Ptr message) {
    std::string currentLanguage = UserStorage::Instance()[message->chat->id]->language;
    sendMessage(bot, message->chat->id, dialogSelectLanguage(currentLanguage), 0, keyboard);
}
