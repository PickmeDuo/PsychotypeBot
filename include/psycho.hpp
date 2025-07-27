#ifndef PSYCHO_HPP
#define PSYCHO_HPP

#include <tgbot/tgbot.h>

#include <string>
#include <vector>
#include <map>

namespace Psycho {
    class PsychologicalAnalyzerBot {
    public:
        explicit PsychologicalAnalyzerBot(const std::string& token);

        // Копирование запрещено
        PsychologicalAnalyzerBot(const PsychologicalAnalyzerBot&) = delete;
        PsychologicalAnalyzerBot& operator=(const PsychologicalAnalyzerBot&) = delete;

        ~PsychologicalAnalyzerBot();

        // Запуск бота и всех обработчиков
        void run();

    private:
        // Основной объект бота от TgBot
        TgBot::Bot bot;

        // Склонность каждого пользователя к различным психотипам.
        // Ключ — username, значение — map<психотип, количество слов склоняющих к психотипу>.
        std::map<std::string, std::map<std::string, int>> tendency;

        // Главный словарь: ключ — название психотипа, 
        // значение — map<ключевое слово, "вес" (важность или частота)>.
        std::map<std::string, std::map<std::string, int>> psychotypeKeywords;

        // Парсит строку сообщения на отдельные слова.
        // убирает знаки препинания, приводит к нижнему регистру и т.п.
        std::vector<std::string> parseMessage(const std::string& text);

        // Преобразует результат анализа (название психотипа) в "человеческое" описание.
        std::string resultDescription(const std::string& psychotype);

        // Анализ собранных данных по человеку с именем 'name'.
        // Возвращает название наиболее вероятного психотипа.
        std::string analyzePerson(const std::string& name);

        // ????
        std::string analyzeGroup(const std::string& name);

        // Обработчик сообщений в группе.
        // Вызывается при каждом новом сообщении.
        void onGroupMessage(TgBot::Message::Ptr message);

        // Обработчик сообщений в личку боту.
        // Например: запрос результатов анализа.
        void onPrivateMessage(TgBot::Message::Ptr message);
    };

} 

#endif