#include "psycho.hpp"
#include <iostream>

using json = nlohmann::json;

void save_data(const std::map<std::string, std::map<std::string, int>>& data) {
    json j = data; // Автоматическое преобразование
    std::ofstream("tendency.json") << j.dump(4); // 4 - отступы для читаемости
}

std::map<std::string, std::map<std::string, int>> load_data() {
    std::ifstream file("tendency.json");
    if (!file) return {}; // Если файла нет
    
    json j;
    file >> j;
    return j.get<std::map<std::string, std::map<std::string, int>>>();
}

//Объявление бота и все инструкции
Psycho::PsychologicalAnalyzerBot::PsychologicalAnalyzerBot(const std::string& token) 
    : bot(std::make_unique<TgBot::Bot>(token)) {

    tendency = load_data();
    
    // Настройка обработчиков

    //Команда /start
    bot->getEvents().onCommand("start", [this](TgBot::Message::Ptr msg) {
        if (msg->chat->type == TgBot::Chat::Type::Group) {
            return;
        }
        auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

        // Первый ряд кнопок (2 кнопки)
        std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
        TgBot::InlineKeyboardButton::Ptr btn1(new TgBot::InlineKeyboardButton);
        btn1->text = "Данные по человеку";
        btn1->callbackData = "btn1";
        row1.push_back(btn1);
        
        TgBot::InlineKeyboardButton::Ptr btn2(new TgBot::InlineKeyboardButton);
        btn2->text = "Данные по группе";
        btn2->callbackData = "btn2";
        row1.push_back(btn2);
        keyboard->inlineKeyboard.push_back(row1);
        
        // Второй ряд кнопок (1 кнопка)
        std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
        TgBot::InlineKeyboardButton::Ptr btn3(new TgBot::InlineKeyboardButton);
        btn3->text = "Сохранить Данные";
        btn3->callbackData = "btn3";
        row2.push_back(btn3);
        keyboard->inlineKeyboard.push_back(row2);
        
        // Настройки предпросмотра ссылок (новый обязательный параметр)
        auto linkPreviewOptions = std::make_shared<TgBot::LinkPreviewOptions>();
        linkPreviewOptions->isDisabled = true; // Выключаем предпросмотр

        // 4. Сущности сообщения (пустой вектор по умолчанию)
        std::vector<TgBot::MessageEntity::Ptr> entities;

        // 5. Отправка сообщения
        bot->getApi().sendMessage(
            msg->chat->id,                // chat_id
            "Что сегодня хочет Сашуля?",  // text
            linkPreviewOptions,           // link_preview_options
            0,                            // reply_to_message_id
            keyboard,                     // reply_markup
            "HTML",                       // parse_mode
            false,                        // disable_notification
            entities,                     // entities (новый параметр вместо message_thread_id)
            false                         // protect_content
        );
    });

    // Команда /person [username]
    bot->getEvents().onCommand("person", [this](TgBot::Message::Ptr message) {
        // Проверяем, что команда имеет аргумент
        if (message->text.empty() || message->text.find(' ') == std::string::npos) {
            bot->getApi().sendMessage(message->chat->id, "Использование: /person [username]");
            return;
        }

        // Извлекаем username из команды
        std::string username = message->text.substr(message->text.find(' ') + 1);
        
        // Удаляем возможные @ в начале
        if (!username.empty() && username[0] == '@') {
            username = username.substr(1);
        }

        // Вызываем анализ пользователя
        std::string analysisResult = analyzePerson(username);
        bot->getApi().sendMessage(message->chat->id, "@" + username + " " + analysisResult);
    });


    // Обработчик нажатий кнопок
    bot->getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
        std::string response;
        
        // Обработка разных callbackData
        if (query->data == "btn1") {
            response = "Напиши /person [username]";
        } 
        else if (query->data == "btn2") {
            response = query->from->firstName + "Сегодня солнечно, +25°C!";
        } 
        else if (query->data == "btn3") {
            save_data(tendency);
            response = "Готово! Данные сохранены.";
        }
        else {
            response = "Неизвестная команда";
        }

        // Отправляем ответ
        bot->getApi().sendMessage(
            query->message->chat->id,
            response
        );
        
        // Убираем "часики" на кнопке (подтверждаем обработку)
        bot->getApi().answerCallbackQuery(query->id);
    });





    //Любое текстовое сообщение
    bot->getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {

        // Проверяем, что сообщение текстовое и из группы, и не от бота
        //if (message->text.empty() || message->chat->type != TgBot::Chat::Type::Group || message->from->isBot) {
        //    return;
        //}

        // Вызываем наш анализатор
        this->onGroupMessage(message->text, message->from->username);
        
        
        bot->getApi().sendMessage(message->chat->id, message->from->username + " сказал '" + message->text + "'");
    });
    //...
}

void Psycho::PsychologicalAnalyzerBot::start() {
    try {
        TgBot::TgLongPoll longPoll(*bot);
        while (true) {
            longPoll.start();
        }
    } catch (const TgBot::TgException& e) {
        std::cerr << "Bot error: " << e.what() << std::endl;
    }
}

int main() {

    Psycho::PsychologicalAnalyzerBot psycho_bot("8135955950:AAG4tPbLm3aWbdwL90alYbrQGBzBG9Y0HhM"); // ВСТАВИТЬ ПИКМИ ТОКЕН

    psycho_bot.start();
    
    return 0;
}