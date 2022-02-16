#pragma once

#include <map>
#include <functional>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestMethod.hpp"
#include <tuple>
#include <regex>
#include <string_view>

/**
 * @brief Http服务链接注册器
 *
 */
class HttpRegisterServer {
protected:
    /**
     * @brief Get服务映射表
     *
     */
    std::map<std::string, std::tuple<std::regex, std::function<void(HttpRequest &, HttpResponse &)>>> registerGetMap;
    /**
     * @brief Post服务映射表
     *
     */
    std::map<std::string, std::tuple<std::regex, std::function<void(HttpRequest &, HttpResponse &)>>> registerPostMap;

protected:
    /**
     * @brief 执行功能体
     *
     * @param request 请求体智能指针
     * @param response 应答体智能指针
     * @param funMap 回调映射
     */
    static void Execute(std::shared_ptr<HttpRequest> request, const std::shared_ptr<HttpResponse> response,
                        std::map<std::string, std::tuple<std::regex, std::function<void(HttpRequest &,
                                                                                        HttpResponse &)>>> &funMap) {
        std::string_view sv(request->GetUrl());
        size_t index;
        if ((index = sv.find('?')) != -1) {
            sv = std::string_view(request->GetUrl().c_str(), index);
        }
        for (auto &&i : funMap) {
            [&](std::pair<const std::string, std::tuple<std::regex, std::function<void(HttpRequest &,
                                                                                       HttpResponse &)>>> &index) {
                std::smatch results;
                std::string urlRegex(sv);
                if (std::regex_match(urlRegex, results, std::get<0>(index.second))) {
                    std::get<1>(index.second)(*request, *response);
                }
            }(i);
        }
    }

public:
    /**
     * @brief 注册Get
     *
     * @param url 请求链接
     * @param callback 请求结果回调
     */
    void RegisterGet(const std::string &url, const std::function<void(HttpRequest &, HttpResponse &)> &callback) {
        registerGetMap.insert(std::make_pair(url,
                                             std::tuple<std::regex, std::function<void(HttpRequest &, HttpResponse &)>>(
                                                     std::regex(url), callback)));
    }

    /**
     * @brief 注册Post
     *
     * @param url 请求链接
     * @param callback 请求结果回调
     */
    void RegisterPost(const std::string &url, const std::function<void(HttpRequest &, HttpResponse &)> &callback) {
        registerPostMap.insert(std::make_pair(url,
                                              std::tuple<std::regex, std::function<void(HttpRequest &,
                                                                                        HttpResponse &)>>(
                                                      std::regex(url), callback)));
    }

    /**
     * @brief 映射请求
     *
     * @param request 请求对象智能指针
     * @param response 应答对象智能指针
     */
    void MapRequest(const std::shared_ptr<HttpRequest> &request, const std::shared_ptr<HttpResponse> &response) {
        switch (request->GetMethod()) {
            case RequestMethod::GET:
                Execute(request, response, registerGetMap);
                break;
            case RequestMethod::POST:
                Execute(request, response, registerPostMap);
                break;
        }
    }
};