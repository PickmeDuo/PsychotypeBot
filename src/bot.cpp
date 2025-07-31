#include "psycho.hpp"
#include <iostream>

const int64_t SenyaId = 600103789;
const int64_t SashaId = 859762063;
const int64_t PickmeDuoId = -1002498428635;
const int64_t _241Id = -1002204012992;
const int64_t _242Id = -1002177162783;

const int SAVEMAX = 20;
int saveCnt = 0;

using json = nlohmann::json;




void save_data_tendency(const std::map<std::string, std::map<std::string, int>>& data) {
    json j = data; // Автоматическое преобразование
    std::ofstream("tendency.json") << j.dump(4); // 4 - отступы для читаемости
}

std::map<std::string, std::map<std::string, int>> load_data_tendency() {
    std::ifstream file("tendency.json");
    if (!file) return {}; // Если файла нет
    
    json j;
    file >> j;
    return j.get<std::map<std::string, std::map<std::string, int>>>();
}


void save_data_groupsData(const std::map<int64_t, std::vector<std::string>>& data) {
    json j = data; // Автоматическое преобразование
    std::ofstream("groupsData.json") << j.dump(4); // 4 - отступы для читаемости
}

std::map<int64_t, std::vector<std::string>> load_data_groupsData() {
    std::ifstream file("groupsData.json");
    if (!file) return {}; // Если файла нет
    
    json j;
    file >> j;
    return j.get<std::map<int64_t, std::vector<std::string>>>();
}




//Объявление бота и все инструкции
Psycho::PsychologicalAnalyzerBot::PsychologicalAnalyzerBot(const std::string& token) 
    : bot(std::make_unique<TgBot::Bot>(token)) {

    tendency = load_data_tendency();
    groupsData = load_data_groupsData();
    
    // Настройка обработчиков



    //Команда /save
    bot->getEvents().onCommand("save", [this](TgBot::Message::Ptr msg) {
        save_data_tendency(tendency);
        save_data_groupsData(groupsData);
        bot->getApi().sendMessage(msg->chat->id, "Готово! Данные сохранены.");
    });



    //Команда /start
    bot->getEvents().onCommand("start", [this](TgBot::Message::Ptr msg) {
        /*
        if (!(msg->from->id == SenyaId || msg->from->id == SashaId)) {
            bot->getApi().sendMessage(msg->chat->id, "Мной могут командовать только Сеня и Саша.");
            return;
        }
        */
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
        btn3->text = "Какие есть психотипы?";
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
            "Че думаете",  // text
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
        if (!(message->from->id == SenyaId || message->from->id == SashaId)) {
            bot->getApi().sendMessage(message->chat->id, "Мной могут командовать только Сеня и Саша.");
            return;
        }

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

    //Команда /group [groupname]
    bot->getEvents().onCommand("group", [this](TgBot::Message::Ptr message) {
        if (!(message->from->id == SenyaId || message->from->id == SashaId)) {
            bot->getApi().sendMessage(message->chat->id, "Мной могут командовать только Сеня и Саша.");
            return;
        }

        // Проверяем, что команда имеет аргумент
        if (message->text.empty() || message->text.find(' ') == std::string::npos) {
            bot->getApi().sendMessage(message->chat->id, "Использование: /group [groupname]");
            return;
        }

        // Извлекаем groupname из команды
        std::string groupname = message->text.substr(message->text.find(' ') + 1);

        for (const auto& [id, vect] : groupsData) {
            std::string chat_name = bot->getApi().getChat(id)->title;
            if (chat_name == groupname) {
                // Вызываем анализ группы
                std::string analysisResult = analyzeGroup(id);
                bot->getApi().sendMessage(message->chat->id, groupname + ":\n" + analysisResult);
                return;
            }
        }
        bot->getApi().sendMessage(message->chat->id, groupname + ":\n" + "Такой группы не существует или я в такой не состаю.");
    });

    //Команда /241
    bot->getEvents().onCommand("241", [this](TgBot::Message::Ptr message) {
        if (!(message->from->id == SenyaId || message->from->id == SashaId)) {
            bot->getApi().sendMessage(message->chat->id, "Мной могут командовать только Сеня и Саша.");
            return;
        }

        std::string analysisResult = analyzeGroup(_241Id);
        bot->getApi().sendMessage(message->chat->id, "241:\n" + analysisResult);
    });

    //Команда /242
    bot->getEvents().onCommand("242", [this](TgBot::Message::Ptr message) {
        if (!(message->from->id == SenyaId || message->from->id == SashaId)) {
            bot->getApi().sendMessage(message->chat->id, "Мной могут командовать только Сеня и Саша.");
            return;
        }

        std::string analysisResult = analyzeGroup(_242Id);
        bot->getApi().sendMessage(message->chat->id, "242:\n" + analysisResult);
    });







    // Обработчик нажатий кнопок
    bot->getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
        std::string response;
        
        // Обработка разных callbackData

        // Данные по человеку
        if (query->data == "btn1") {
            if (!(query->from->id == SenyaId || query->from->id == SashaId)) {
                bot->getApi().sendMessage(query->message->chat->id, "Мной могут командовать только Сеня и Саша.");
                return;
            }
            /*
            if (query->message->chat->id == PickmeDuoId) {
                response = "Напиши /person [username]";
            }
            */
            else {
                response = "@" + query->from->username + " " + analyzePerson(query->from->username);
            }
        }
        // Данные по группе
        else if (query->data == "btn2") {
            if (!(query->from->id == SenyaId || query->from->id == SashaId)) {
                bot->getApi().sendMessage(query->message->chat->id, "Мной могут командовать только Сеня и Саша.");
                return;
            }
            if (query->message->chat->id == PickmeDuoId) {
                response = "Напиши /group [groupname]\n Список доступных групп:\n";
                for (const auto& [id, vect] : groupsData) {
                    std::string chat_name = bot->getApi().getChat(id)->title;
                    response += chat_name + "\n";
                }
                response += "\nВаша группа:\n";
                response += analyzeGroup(query->message->chat->id);
            }
            else {
                response = analyzeGroup(query->message->chat->id);
            }
        } 
        // Какие есть психотипы
        else if (query->data == "btn3") {


            // Инлайн клавиатура для различных психотипов
            // 9 кнопок:

            auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

            // Первый ряд кнопок (3 кнопки)
            std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
            TgBot::InlineKeyboardButton::Ptr btn1(new TgBot::InlineKeyboardButton);
            btn1->text = "Пикми";
            btn1->callbackData = "type1";
            row1.push_back(btn1);
            
            TgBot::InlineKeyboardButton::Ptr btn2(new TgBot::InlineKeyboardButton);
            btn2->text = "Истероид";
            btn2->callbackData = "type2";
            row1.push_back(btn2);

            TgBot::InlineKeyboardButton::Ptr btn3(new TgBot::InlineKeyboardButton);
            btn3->text = "Эпилептоид";
            btn3->callbackData = "type3";
            row1.push_back(btn3);
            keyboard->inlineKeyboard.push_back(row1);
            
            // Второй ряд кнопок (3 кнопки)
            std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
            TgBot::InlineKeyboardButton::Ptr btn4(new TgBot::InlineKeyboardButton);
            btn4->text = "Шизоид";
            btn4->callbackData = "type4";
            row2.push_back(btn4);
            
            TgBot::InlineKeyboardButton::Ptr btn5(new TgBot::InlineKeyboardButton);
            btn5->text = "Гипертим";
            btn5->callbackData = "type5";
            row2.push_back(btn5);

            TgBot::InlineKeyboardButton::Ptr btn6(new TgBot::InlineKeyboardButton);
            btn6->text = "Параноял";
            btn6->callbackData = "type6";
            row2.push_back(btn6);
            keyboard->inlineKeyboard.push_back(row2);

            // Третий ряд кнопок (3 кнопки)
            std::vector<TgBot::InlineKeyboardButton::Ptr> row3;
            TgBot::InlineKeyboardButton::Ptr btn7(new TgBot::InlineKeyboardButton);
            btn7->text = "Эмотив";
            btn7->callbackData = "type7";
            row3.push_back(btn7);
            
            TgBot::InlineKeyboardButton::Ptr btn8(new TgBot::InlineKeyboardButton);
            btn8->text = "Тревожно-мнительный";
            btn8->callbackData = "type8";
            row3.push_back(btn8);

            TgBot::InlineKeyboardButton::Ptr btn9(new TgBot::InlineKeyboardButton);
            btn9->text = "Депрессивно-Печальный";
            btn9->callbackData = "type9";
            row3.push_back(btn9);
            keyboard->inlineKeyboard.push_back(row3);
            
            // Настройки предпросмотра ссылок (новый обязательный параметр)
            auto linkPreviewOptions = std::make_shared<TgBot::LinkPreviewOptions>();
            linkPreviewOptions->isDisabled = true; // Выключаем предпросмотр

            // 4. Сущности сообщения (пустой вектор по умолчанию)
            std::vector<TgBot::MessageEntity::Ptr> entities;

            // 5. Отправка сообщения
            bot->getApi().sendMessage(
                query->message->chat->id,         // chat_id
                "Про какой психотип рассказать",  // text
                linkPreviewOptions,               // link_preview_options
                0,                                // reply_to_message_id
                keyboard,                         // reply_markup
                "HTML",                           // parse_mode
                false,                            // disable_notification
                entities,                         // entities (новый параметр вместо message_thread_id)
                false                             // protect_content
            );
            return;
        }
        // Действия на эти кнопки
        else if (query->data == "type1") {
            response = resultDescription("Пикми");
        }
        else if (query->data == "type2") {
            response = resultDescription("Истероид");
        }
        else if (query->data == "type3") {
            response = resultDescription("Эпилептоид");
        }
        else if (query->data == "type4") {
            response = resultDescription("Шизоид");
        }
        else if (query->data == "type5") {
            response = resultDescription("Гипертим");
        }
        else if (query->data == "type6") {
            response = resultDescription("Параноял");
        }
        else if (query->data == "type7") {
            response = resultDescription("Эмотив");
        }
        else if (query->data == "type8") {
            response = resultDescription("Тревожно-Мнительный");
        }
        else if (query->data == "type9") {
            response = resultDescription("Депрессивно-Печальный");
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





    // Любое текстовое сообщение
    bot->getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {

        //log
        std::cout << message->from->username << ": " << message->text << "\n";

        // Вызываем наш анализатор
        onGroupMessage(message->text, message->from->username, message->chat->id);

        // Кто-то пишет в личку бота
        if (message->chat->type == TgBot::Chat::Type::Private) {
            std::string username = message->from->username;
            // Рассылаем по всем чатам, в которых этот чел есть
            for (const auto& [id, vect] : groupsData) {
                if (id == message->chat->id || id == PickmeDuoId) {
                    continue;
                }
                for (const auto& us : vect) {
                    if (us == username && !message->text.empty()) {
                        bot->getApi().sendMessage(id, message->text);
                    }
                }
            }
            // Дублируем сообщение в наш пикми чатик
            bot->getApi().sendMessage(PickmeDuoId, "Лс бота: " + username + ": " + message->text);
        }

        //Раз в SAVEMAX сообщений сохраняем данные
        ++saveCnt;
        if (saveCnt == SAVEMAX) {
            save_data_tendency(tendency);
            save_data_groupsData(groupsData);
            saveCnt = 0;
            std::cout << "Data Saved.\n";
        }
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