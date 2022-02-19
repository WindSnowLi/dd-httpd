#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <sstream>
#include <string_view>
#include <utility>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestMethod.hpp"
#include "HttpRegisterInterceptor.hpp"
#include "HttpRegisterServer.hpp"
#include "Utils.hpp"
#include "ThreadPool.hpp"

/**
 * @brief 基础服务部分
 *
 */
class HttpServer {
public:
    /**
     * @brief 线程池
     *
     */
    std::shared_ptr<ThreadPool> threadPool;

    /**
     * @brief Http拦截器注册器
     *
     */
    std::shared_ptr<HttpRegisterInterceptor> httpRegisterInterceptor;
    /**
     * @brief Http服务注册器
     *
     */
    std::shared_ptr<HttpRegisterServer> httpRegisterServer;

protected:
    /**
     * @brief
     *
     * @param client 网络适配器
     * @return std::shared_ptr<HttpRequest>
     */
    static std::shared_ptr<HttpRequest> Parse(const std::shared_ptr<NetworkAdapter>& client) {
        std::stringstream &&ss = client->Read();
        std::string line{};
        getline(ss, line);
        std::shared_ptr<HttpRequest> request = [&]() {
            auto r = std::make_shared<HttpRequest>();
            std::string_view sv1(line);
            size_t first = sv1.find(' ');
            r->SetMethod(StrToEnum(std::string(sv1.substr(0, first))));

            std::string_view sv2(line.c_str() + first + 1);
            size_t second = sv2.find(' ');
            r->SetUrl(std::string(sv2.substr(0, second)));

            std::string_view sv3(line.c_str() + first + 1 + second + 1);
            r->SetProtocol(std::string(sv3.substr(0)));
            return r;
        }();
        size_t methodIndex = request->GetUrl().find('?');
        if (methodIndex != -1) {
            std::vector<std::string_view> &&params = StrUtil::Split({request->GetUrl().c_str() + methodIndex + 1}, '&');
            request->SetParams(StrUtil::PropertyParse(params));
        }

        getline(ss, line);
        while (!line.empty()) {
            size_t split = line.find(':');
            request->AddHeader(line.substr(0, split), line.substr(split + 1));
            getline(ss, line);
        }
        std::string body{};
        while (!ss.eof()) {
            getline(ss, line);
            body += line;
        }

        request->SetBody(body);
        return request;
    }

    /**
     * @brief 应答数据
     *
     * @param client 网络适配器
     * @param response 应答体
     */
    static void Response(const std::shared_ptr<NetworkAdapter>& client, std::shared_ptr<HttpResponse> response) {
        client->Write(response->GetHttpPackage());
    }

public:
    /**
     * @brief 开始执行Http交互
     *
     * @param client 网络适配器
     */
    void AcceptHttp(std::shared_ptr<NetworkAdapter> client) {
        threadPool->AddTask(
                [h = httpRegisterInterceptor, s = httpRegisterServer, c = std::move(client)]() {
                    auto request = Parse(c);
                    std::shared_ptr<HttpResponse> response = std::make_shared<HttpResponse>();
                    // 验证前置拦截器
                    if (h != nullptr && h->VerifyBefore(request, response)) {
                        return;
                    }
                    // 映射请求
                    s == nullptr ? []() {}() : s->MapRequest(request, response);
                    // 验证后置拦截器
                    if (h != nullptr && h->VerifyAfter(request, response)) {
                        return;
                    }
                    Response(c, response);
                });
    }
};
