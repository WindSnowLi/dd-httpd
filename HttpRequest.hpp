#pragma once

#include <string>

#include "RequestMethod.hpp"
#include "BaseInfo.hpp"

class HttpRequest
{
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

public:
    /**
     * @brief Get the Method object
     *
     * @return const RequestMethod& 请求方法
     */
    const RequestMethod &GetMethod() const
    {
        return requestMethod;
    }
    /**
     * @brief Set the Method object
     *
     * @param r
     */
    void SetMethod(RequestMethod r)
    {
        this->requestMethod = r;
    }

    /**
     * @brief Get the Url object
     *
     * @return const std::string&
     */
    const std::string &GetUrl() const
    {
        return url;
    }

    void SetUrl(const std::string &u)
    {
        this->url = u;
    }

    void SetProtocol(const std::string &p)
    {
        this->protocol = p;
    }

    const std::string &GetProtocol() const
    {
        return protocol;
    }

    const std::string &GetBody() const
    {
        return body;
    }

    void SetBody(const std::string &str)
    {
        this->body = str;
    }

    void AddHeader(const std::string &key, const std::string &value)
    {
        this->header.insert(std::make_pair(key, value));
    }

    const std::string &GetHeader(const std::string &key) const
    {
        return this->header.at(key);
    }

    void SetHeaderMap(const std::map<std::string, std::string> &map)
    {
        this->header = map;
    }

    const std::map<std::string, std::string> &GetHeaderMap() const
    {
        return this->header;
    }
};
