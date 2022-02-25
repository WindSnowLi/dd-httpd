#pragma once

#include <string>

/**
 * @brief 请求方法
 * 
 */
enum class RequestMethod {
    GET,
    POST,
    HEAD
};

inline std::string EnumToStr(RequestMethod r) {
    switch (r) {
        case RequestMethod::GET:
            return "GET";
        case RequestMethod::POST:
            return "POST";
        case RequestMethod::HEAD:
            return "HEAD";
    }
}

inline RequestMethod StrToEnum(const std::string &str) {
    if (str == "POST") {
        return RequestMethod::POST;
    } else if (str == "HEAD") {
        return RequestMethod::HEAD;
    } else {
        return RequestMethod::GET;
    }
}