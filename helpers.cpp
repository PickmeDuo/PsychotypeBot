#include "psycho.hpp"

#include <cctype>
#include <sstream>

namespace Psycho {
    std::vector<std::string> PsychologicalAnalyzerBot::parseMessage(const std::string& text) {
        std::string clear;
        clear.reserve(text.size());

        //проходим по символам, пробел или буква -> 
        // -> приводим к нижнему и кладём в clear

        for (char c : text) {
            if (std::isalpha(static_cast<unsigned char>(c)) || 
            std::isspace(static_cast<unsigned char>(c))) {
                clear += std::tolower(static_cast<unsigned char>(c));
            }
        }

        std::vector<std::string> words;
        std::istringstream iss(clear); //разбиваем по пробелам
        std::string word;

        while (iss >> word) {
            words.push_back(word); //добавляем каждое слово в вектор
        }

        return words;
    }
}