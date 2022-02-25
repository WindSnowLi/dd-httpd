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
    std::stringstream rsp{};
    bool package = true;
    RequestMethod requestMethod;
public:
    RequestMethod getRequestMethod() const {
        return requestMethod;
    }

    void SetRequestMethod(RequestMethod method) {
        HttpResponse::requestMethod = method;
    }

public:

    HttpResponse() = default;

    HttpResponse(const HttpResponse &httpResponse) : protocol(httpResponse.protocol), code(httpResponse.code),
                                                     header(httpResponse.header),
                                                     body(httpResponse.body),
                                                     package(true),
                                                     requestMethod(httpResponse.requestMethod) {

    }

    size_t getCode() const {
        return code;
    }

    void setCode(size_t c) {
        this->code = c;
    }

    const std::string &GetProtocol() const {
        return protocol;
    }

    size_t GetCode() const {
        return code;
    }

    void AddHeader(const std::string &key, const std::string &value) {
        this->header.insert(std::make_pair(key, value));
        package = true;
    }

    const std::string &GetHeader(const std::string &key) const {
        return this->header.at(key);
    }

    void SetHeaderMap(const std::map<std::string, std::string> &map) {
        this->header = map;
        package = true;
    }

    const std::map<std::string, std::string> &GetHeaderMap() const {
        return this->header;
    }

    const std::string &GetBody() const {
        return body;
    }

    void SetBody(const std::string &str) {
        this->body = str;
        package = true;
    }

    const std::string &GetDesc() const {
        return desc;
    }

    void SetDesc(const std::string &str) {
        this->desc = str;
        package = true;
    }

    const std::stringstream &GetHttpPackage() {
        if (package) {
            rsp.clear();
            rsp << this->GetProtocol() << ' ' << this->GetCode() << ' ' << this->GetDesc() << "\r\n";
            rsp << CONTENT_LENGTH << this->GetBody().size() << "\r\n";

            for_each(this->GetHeaderMap().begin(),
                     this->GetHeaderMap().end(),
                     [&](const std::pair<std::string, std::string> &index) {
                         rsp << index.first << ':' << index.second << "\r\n";
                     });
            rsp << "\r\n";
            if (this->requestMethod != RequestMethod::HEAD) {
                rsp << this->GetBody();
            }
            package = false;
        }
        return rsp;
    }
};
