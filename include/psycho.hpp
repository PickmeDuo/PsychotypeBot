#ifndef PSYCHO_HPP
#define PSYCHO_HPP

#include "dictionary.hpp"

#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Psycho {
    class PsychologicalAnalyzerBot {
    public:
        explicit PsychologicalAnalyzerBot(const std::string& token);

        // Копирование запрещено
        PsychologicalAnalyzerBot(const PsychologicalAnalyzerBot&) = delete;
        PsychologicalAnalyzerBot& operator=(const PsychologicalAnalyzerBot&) = delete;

        ~PsychologicalAnalyzerBot() = default;

        // Запуск бота и всех обработчиков
        void start();

    private:
        std::unique_ptr<TgBot::Bot> bot;

        // Склонность каждого пользователя к различным психотипам.
        // Ключ — username, значение — map<психотип, количество слов склоняющих к психотипу>.
        std::map<std::string, std::map<std::string, int>> tendency;

        // Информация о группах.
        // Ключ - id группы, значение - вектор всех людей, состоящих в этой группе.
        std::map<int64_t, std::vector<std::string>> groupsData;

        std::pair<unsigned char, unsigned char> toLoweRu(unsigned char c1, unsigned char c2);

        // Парсит строку сообщения на отдельные слова.
        // убирает знаки препинания, приводит к нижнему регистру и т.п.
        std::vector<std::string> parseMessage(const std::string& text);

        // Преобразует результат анализа (название психотипа) в "человеческое" описание.
        std::string resultDescription(const std::string& psychotype);

        // Анализ собранных данных по человеку с именем 'name'.
        // Возвращает название наиболее вероятного психотипа.
        std::string analyzePerson(const std::string& name);

        // Анализ собранных данных по группе
        std::string analyzeGroup(const int64_t chat_id);

        // Обработчик сообщений в группе.Вызывается при каждом новом сообщении.
        // Парсит сообщение message, анализирует и добавляет данные в tendency человеку с ником username.
        void onGroupMessage(const std::string& message, const std::string& username, const int64_t chat_id);
    };

} 

#endif