#pragma once

#include <string>

/**
 * @brief 请求方法
 * 
 */
enum class RequestMethod {
    GET,
    POST
};

inline std::string EnumToStr(RequestMethod r) {
    switch (r) {
        case RequestMethod::GET:
            return "GET";
        case RequestMethod::POST:
            return "POST";
    }
}

inline RequestMethod StrToEnum(const std::string &str) {
    if (str == "GET") {
        return RequestMethod::GET;
    } else if (str == "POST") {
        return RequestMethod::POST;
    } else {
        return RequestMethod::GET;
    }
}