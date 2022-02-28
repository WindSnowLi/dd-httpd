#pragma once

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <string_view>

namespace StrUtils {
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

    void ReplaceChar(std::string &sv, char oldCh, char newCh) {
        for (char &i : sv) {
            if (i == oldCh) {
                i = newCh;
            }
        }
    }

    bool CheckPathDeepin(const std::string_view &path) {
        std::string temp(path);
        ReplaceChar(temp, '\\', '/');
        std::vector<std::string_view> svs = Split((temp), '/');
        int count = 0;
        std::string pre = "..";
        for (auto &&i : svs) {
            if (i.empty()) {
                continue;
            }
            if (i != pre) {
                ++count;
            } else {
                --count;
            }
            if (count < 0) {
                return false;
            }
        }
        return true;
    }

    inline void ToLowCase(std::string& sv) {
        std::transform(sv.begin(), sv.end(), sv.begin(), ::tolower);
    }
}

namespace FileUtils {
    size_t GetFileLength(const std::string &path);

    size_t GetFileStreamLength(std::ifstream &fp);

    size_t GetFileLength(const std::string &path) {
        std::ifstream fp(std::filesystem::u8path(path), std::ios::in);
        if (!fp) {
            return 0;
        }
        size_t length = GetFileStreamLength(fp);
        fp.close();
        return length;
    }

    size_t GetFileStreamLength(std::ifstream &fp) {
        if (!fp) {
            return 0;
        }
        //定位到文件末尾
        fp.seekg(0, std::ifstream::end);
        //获得文件总长度
        size_t length = fp.tellg();
        fp.seekg(0, std::ifstream::beg);
        return length;
    }

    inline bool ExistFile(const std::string &path) {
        return std::filesystem::exists(std::filesystem::u8path(path));
    }
}

namespace DateUtils {
    std::string GetCurrentDate() {
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timeStr = std::ctime(&time);
        timeStr.resize(timeStr.size() - 1);
        return timeStr;
    }
}