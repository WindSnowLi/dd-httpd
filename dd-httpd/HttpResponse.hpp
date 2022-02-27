#pragma once

#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#include "BaseInfo.hpp"

#ifndef NOT_FOUNT
#define NOT_FOUNT 404
#endif

class HttpResponse {
protected:
    std::string protocol{PROTOCOL};
    size_t code = 200;
    std::map<std::string, std::string> header;
    std::string body{};
    std::string desc{};
    RequestMethod requestMethod;
    size_t length = 0;
public:

    HttpResponse() = default;

    HttpResponse(const HttpResponse &httpResponse) : protocol(httpResponse.protocol), code(httpResponse.code),
                                                     header(httpResponse.header),
                                                     body(httpResponse.body),
                                                     requestMethod(httpResponse.requestMethod) {

    }

    [[nodiscard]] size_t GetLength() const {
        return length;
    }

    void setLength(size_t i) {
        HttpResponse::length = i;
    }

    [[nodiscard]] size_t GetCode() const {
        return code;
    }

    void setCode(size_t c) {
        this->code = c;
    }

    [[nodiscard]] const std::string &GetProtocol() const {
        return protocol;
    }

    [[maybe_unused]] void AddHeader(const std::string &key, const std::string &value) {
        this->header.insert(std::make_pair(key, value));
    }

    [[maybe_unused]] [[nodiscard]] const std::string &GetHeader(const std::string &key) const {
        return this->header.at(key);
    }

    [[maybe_unused]] void SetHeaderMap(const std::map<std::string, std::string> &map) {
        this->header = map;
    }

    [[nodiscard]] const std::map<std::string, std::string> &GetHeaderMap() const {
        return this->header;
    }

    [[nodiscard]] const std::string &GetBody() const {
        return body;
    }

    void SetBody(const std::string &str) {
        this->body = str;
        this->length = this->body.size();
    }

    [[maybe_unused]] [[nodiscard]] RequestMethod GetRequestMethod() const {
        return requestMethod;
    }

    void SetRequestMethod(RequestMethod method) {
        HttpResponse::requestMethod = method;
    }

    [[nodiscard]] const std::string &GetDesc() const {
        return desc;
    }

    [[maybe_unused]] void SetDesc(const std::string &str) {
        this->desc = str;
    }

    [[nodiscard]] std::stringstream GetHeadStream() const {
        std::stringstream rsp{};
        rsp << GetProtocol() << ' '
            << GetCode() << ' '
            << GetDesc() << "\r\n"
            << CONTENT_LENGTH << ':'
            << GetLength() << "\r\n";
        for_each(GetHeaderMap().begin(),
                 GetHeaderMap().end(),
                 [&](const std::pair<std::string, std::string> &index) {
                     rsp << index.first << ':' << index.second << "\r\n";
                 });
        rsp << "\r\n";
        return rsp;
    }
};

