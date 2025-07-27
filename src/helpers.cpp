#include "psycho.hpp"

#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <utility>

namespace Psycho {
    std::vector<std::string> PsychologicalAnalyzerBot::parseMessage(const std::string& text) {
        std::string clear;
        clear.reserve(text.size());

        //проходим по символам, буква -> 
        // -> приводим к нижнему и кладём в clear

        for (char c : text) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                clear += std::tolower(static_cast<unsigned char>(c));
            } else {
                clear += ' ';
            }

        std::vector<std::string> words;
        std::istringstream iss(clear); //разбиваем по пробелам
        std::string word;

        while (iss >> word) {
            words.push_back(word); //добавляем каждое слово в вектор
        }

        return words;
    }

    std::string PsychologicalAnalyzerBot::analyzePerson(const std::string& name) {
        auto userId = tendency.find(name);

        if (userId == tendency.end()) {
            return "Нет данных для" + name;
        }

        const auto& userPsycho = userId->second;

        if (userPsycho.empty()) {
            return "Недостаточно информации для" + name;
        }

        std::vector<std::pair<std::string, int>> sorted(userPsycho.begin(), userPsycho.end());

        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });
        
        std::vector<std::string> resultPsycho;

        int max1 = sorted[0].second;
        int max2 = sorted[1].second;

        for (const auto& [type, score] : sorted) {
            if (score == max1) {
                resultPsycho.push_back(type);
            }
        }

        if ((max1 - max2) < (max1 / 10)) {
            for (const auto& [type, score] : sorted) {
                if (score == max2) {
                    resultPsycho.push_back(type);
                }
            }
        } 

        std::string result;
        int n = static_cast<int> resultPsycho.size();

        for (int i = 0; i < n; ++i) {
            result += resultPsycho[i];
            if (i + 2 == n) {
                result += " и ";
            } else if (i + 1 < n) {
                result += ", ";
            }
        }

        return result;
    } 

    std::string PsychologicalAnalyzerBot::analyzeGroup(std::map<std::string, std::map<std::string, int>>& tendency) {
        if (tendency.empty()) {
            return "Нет данных для анализа"
        }

        std::string result;

        for (const auto& [name, score] : tendency) {
            std::string psycho == analyzePerson(name);
            result += name + ": " + psycho + "\n";
        }

        return result;
    }

}