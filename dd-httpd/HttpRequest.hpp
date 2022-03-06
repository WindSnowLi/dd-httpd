#pragma once

#include <string>
#include <filesystem>

#include "RequestMethod.hpp"
#include "BaseInfo.hpp"

class HttpRequest {
protected:
    /**
     * @brief 请求方法
     *
     */
    RequestMethod requestMethod;

    /**
     * @brief 请求URL
     *
     */
    std::string url;
    /**
     * @brief 协议
     *
     */
    std::string protocol{PROTOCOL};
    /**
     * @brief 请求头表
     *
     */
    std::map<std::string, std::string> header;
    /**
     * @brief 请求体
     *
     */
    std::string body{};


    std::map<std::string, std::string> params;

    std::string filePath{};

    std::vector<std::pair<long long, long long>> partTable;
public:
    [[nodiscard]] RequestMethod GetRequestMethod() const {
        return requestMethod;
    }

    void SetRequestMethod(RequestMethod method) {
        HttpRequest::requestMethod = method;
    }

    [[nodiscard]] const std::string &GetUrl() const {
        return url;
    }

    void SetUrl(const std::string &u) {
        HttpRequest::url = u;
    }

    [[maybe_unused]] [[nodiscard]] const std::string &GetProtocol() const {
        return protocol;
    }

    void SetProtocol(const std::string &p) {
        protocol = p;
    }

    [[maybe_unused]] [[nodiscard]] std::map<std::string, std::string> &GetHeader() {
        return header;
    }

    [[maybe_unused]] void SetHeader(const std::map<std::string, std::string> &map) {
        HttpRequest::header = map;
    }

    [[nodiscard]] const std::string &GetBody() const {
        return body;
    }

    void SetBody(const std::string &b) {
        HttpRequest::body = b;
    }

    [[nodiscard]] const std::map<std::string, std::string> &GetParams() const {
        return params;
    }

    void SetParams(const std::map<std::string, std::string> &map) {
        HttpRequest::params = map;
    }

    std::string &GetFilePath() {
        return filePath;
    }

    void SetFilePath(const std::string &path) {
        HttpRequest::filePath = path;
    }

    void AddHeader(const std::string &key, const std::string &value) {
        this->header.insert(std::make_pair(key, value));
    }

    void SetPartTable(const std::vector<std::pair<long long, long long>>& t) {
        this->partTable = t;
    }
    const std::vector<std::pair<long long, long long>>& GetPartTable() {
        return this->partTable;
    }
};
