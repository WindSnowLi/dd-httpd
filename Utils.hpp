#pragma once

namespace StrUtil {
    std::vector<std::string_view> Split(std::string_view sv, char ch) {
        std::vector<std::string_view> points;
        size_t point = 0;
        for (size_t i = 0; i < sv.length(); i++) {
            if (sv[i] == ch) {
                points.emplace_back(std::string_view(&sv[point], i - point));
                point = i + 1;
            }
        }
        points.emplace_back(std::string_view(&sv[point], sv.length() - point));
        return points;
    }

    std::map<std::string, std::string> PropertyParse(const std::vector<std::string_view> &svs) {
        std::map<std::string, std::string> property;
        std::string buff;
        for (const std::string_view &i : svs) {
            std::vector<std::string_view> &&table = Split(i, '=');
            if (table.size() < 2) {
                continue;
            }
            for (size_t j = 1; j < table.size(); ++j) {
                buff += table[j];
            }
            property.insert(std::make_pair(table[0], buff));
            buff.clear();
        }
        return property;
    }

}