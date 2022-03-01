#pragma once

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "Utils.hpp"
#include "BaseInfo.hpp"

class HttpResponse {
protected:
    std::string protocol{PROTOCOL};
    size_t code = 404;
    std::map<std::string, std::string> header;
    std::string body{};
    std::string desc{RESPONSE_DESC_NOT_FOUND};
    size_t length = 0;
    std::ifstream fp;
public:

    HttpResponse() = default;

    ~HttpResponse() {
        if (fp.is_open()) {
            fp.close();
        }
    }

    HttpResponse(const HttpResponse &httpResponse) : protocol(httpResponse.protocol), code(httpResponse.code),
                                                     header(httpResponse.header),
                                                     body(httpResponse.body) {

    }

    [[nodiscard]] size_t GetCode() const {
        return code;
    }

    void SetCode(size_t c) {
        this->code = c;
        switch (c) {
            case 200:
                this->desc = RESPONSE_DESC_OK;
                break;
            case 404:
                this->desc = RESPONSE_DESC_NOT_FOUND;
                break;
            default:
                this->desc = RESPONSE_DESC_OK;
        }
    }

    [[nodiscard]] const std::string &GetProtocol() const {
        return protocol;
    }

    [[maybe_unused]] void AddHeader(const std::string &key, const std::string &value) {
        this->header.insert(std::make_pair(key, value));
    }

    [[maybe_unused]] [[nodiscard]] std::map<std::string, std::string>& GetHeader() {
        return this->header;
    }

    [[maybe_unused]] void SetHeaderMap(const std::map<std::string, std::string> &map) {
        this->header = map;
    }

    [[nodiscard]] const std::map<std::string, std::string> &GetHeaderMap() const {
        return this->header;
    }

    [[nodiscard]] const std::string &GetBody() const {
        return this->body;
    }

    void SetBody(const std::string &str) {
        this->body = str;
        this->length = this->body.size();
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
            << this->length << "\r\n"
            << RESPONSE_HEAD_SERVER << ':'
            << SERVER_NAME << "\r\n"
            << RESPONSE_HEAD_DATE << ':'
            << DateUtils::GetCurrentDate() << "\r\n";
        for_each(GetHeaderMap().begin(),
                 GetHeaderMap().end(),
                 [&](const std::pair<std::string, std::string> &index) {
                     rsp << index.first << ':' << index.second << "\r\n";
                 });
        rsp << "\r\n";
        return rsp;
    }

    void SetFp(const std::string &path) {
        if (fp) {
            fp.close();
        }
        fp.open(std::filesystem::u8path(path), std::ios::binary | std::ios::in);
        fp.is_open() ? SetCode(OK) : SetCode(NOT_FOUNT);
        this->length = FileUtils::GetFileStreamLength(fp);
    }

    std::ifstream &GetFp() {
        return fp;
    }
};

