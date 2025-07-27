#include <tgbot/tgbot.h>
#include <iostream>

int main() {
    /*
    // 1. Создаём бота с токеном (получите у @BotFather)
    TgBot::Bot bot("8450803481:AAHmW09KxQyY91batyAuN5XKAtkpJlkWPv0");

    // 2. Команда /start
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        // Создаем клавиатуру с несколькими кнопками
        auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
        
        // Первый ряд кнопок (2 кнопки)
        std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
        TgBot::InlineKeyboardButton::Ptr btn1(new TgBot::InlineKeyboardButton);
        btn1->text = "Нажми меня";
        btn1->callbackData = "btn1";
        row1.push_back(btn1);
        
        TgBot::InlineKeyboardButton::Ptr btn2(new TgBot::InlineKeyboardButton);
        btn2->text = "Котик";
        btn2->callbackData = "btn2";
        row1.push_back(btn2);
        keyboard->inlineKeyboard.push_back(row1);
        
        // Второй ряд кнопок (1 кнопка)
        std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
        TgBot::InlineKeyboardButton::Ptr btn3(new TgBot::InlineKeyboardButton);
        btn3->text = "мимими";
        btn3->callbackData = "btn3";
        row2.push_back(btn3);
        keyboard->inlineKeyboard.push_back(row2);
        
        // Настройки предпросмотра ссылок (новый обязательный параметр)
        auto linkPreviewOptions = std::make_shared<TgBot::LinkPreviewOptions>();
        linkPreviewOptions->isDisabled = true; // Выключаем предпросмотр

        // 4. Сущности сообщения (пустой вектор по умолчанию)
        std::vector<TgBot::MessageEntity::Ptr> entities;

        // 5. Отправка сообщения с ВСЕМИ параметрами
        bot.getApi().sendMessage(
            message->chat->id,           // chat_id
            "Выберите действие:",       // text
            linkPreviewOptions,         // link_preview_options
            0,                          // reply_to_message_id
            keyboard,                   // reply_markup
            "HTML",                     // parse_mode
            false,                      // disable_notification
            entities,                   // entities (новый параметр вместо message_thread_id)
            false                       // protect_content
        );
    });

    // Обработчик нажатий кнопок
    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        std::string response;
        
        // Обработка разных callbackData
        if (query->data == "btn1") {
            response = "Привет, солнышко!";
        } 
        else if (query->data == "btn2") {
            response = "Сегодня солнечно, +25°C!";
        } 
        else if (query->data == "btn3") {
            response = query->from->firstName + ", Я люблю тебя!!!";
        }
        else {
            response = "Неизвестная команда";
        }

        // Отправляем ответ
        bot.getApi().sendMessage(
            query->message->chat->id,
            response
        );
        
        // Убираем "часики" на кнопке (подтверждаем обработку)
        bot.getApi().answerCallbackQuery(query->id);
    });

    // 3. Ответ на любое текстовое сообщение
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (message->text.empty()) return;
        if (message->text[0] != '/') {
            bot.getApi().sendMessage(message->chat->id, "Вы написали: " + message->text);
        }
    });


    // 4. Запуск бота
    try {
        std::cout << "Bot started, name: " << bot.getApi().getMe()->username << "\n";
        TgBot::TgLongPoll longPoll(bot);  // Режим long-polling
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
    */
}