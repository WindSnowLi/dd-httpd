#pragma once

#include <map>
#include <functional>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestMethod.hpp"
#include <tuple>
#include <regex>
#include <string_view>
#include <set>
#include "Utils.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

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

    std::map<std::string, std::tuple<std::regex, std::function<void(HttpRequest &, HttpResponse &)>>> registerHeadMap;

    std::string rootPath{"./"};
public:

    void SetRootPath(const std::string &path) {
        if (path.length() == 0) {
            return;
        }
        this->rootPath = path;
        StrUtil::ReplaceChar(this->rootPath, '\\', '/');
        if (rootPath.back() == '/') {
            rootPath.erase(rootPath.back());
        }
    }

protected:
    /**
     * @brief 执行功能体
     *
     * @param request 请求体智能指针
     * @param response 应答体智能指针
     * @param funMap 回调映射
     */
    static bool Execute(std::shared_ptr<HttpRequest> request, const std::shared_ptr<HttpResponse> response,
                        std::map<std::string, std::tuple<std::regex, std::function<void(HttpRequest &,
                                                                                        HttpResponse &)>>> &funMap) {
        std::string_view sv(request->GetUrl());
        size_t index;
        if ((index = sv.find('?')) != -1) {
            sv = std::string_view(request->GetUrl().c_str(), index);
        }
        for (auto &&i : funMap) {
            if ([&](std::pair<const std::string, std::tuple<std::regex, std::function<void(HttpRequest &,
                                                                                           HttpResponse &)>>> &index) {
                std::smatch results;
                std::string urlRegex(sv);
                if (std::regex_match(urlRegex, results, std::get<0>(index.second))) {
                    std::get<1>(index.second)(*request, *response);
                    return true;
                }
                return false;
            }(i)) {
                return true;
            }
        }
        return false;
    }

    static void GetFile(const std::shared_ptr<HttpRequest> &request, const std::shared_ptr<HttpResponse> &response,
                        const std::string &path) {
        std::string_view sv = request->GetUrl();
        size_t index;
        if ((index = sv.find('?')) != -1) {
            sv = std::string_view(request->GetUrl().c_str(), index);
        }
        request->SetFilePath(path + std::string(sv));
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

    void RegisterHead(const std::string &url, const std::function<void(HttpRequest &, HttpResponse &)> &callback) {
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
        switch (request->GetRequestMethod()) {
            case RequestMethod::GET:
                if (!Execute(request, response, registerGetMap)) {
                    GetFile(request, response, rootPath);
                }
                break;
            case RequestMethod::POST:
                Execute(request, response, registerPostMap);
                break;
            case RequestMethod::HEAD:
                if (!Execute(request, response, registerHeadMap)) {
                    GetFile(request, response, rootPath);
                }
                break;
        }
    }

};