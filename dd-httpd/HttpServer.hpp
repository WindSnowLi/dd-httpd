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
    static std::shared_ptr<HttpRequest> Parse(const std::shared_ptr<NetworkAdapter> &client) {
        auto &&rs = client->Read();
        if (!std::get<1>(rs)) {
            return nullptr;
        }
        std::stringstream &ss = std::get<0>(rs);
        std::string line{};
        getline(ss, line);
        std::shared_ptr<HttpRequest> request = [&]() {
            auto r = std::make_shared<HttpRequest>();
            std::string_view sv1(line);
            size_t first = sv1.find(' ');
            r->SetRequestMethod(StrToEnum(std::string(sv1.substr(0, first))));

            std::string_view sv2(line.c_str() + first + 1);
            size_t second = sv2.find(' ');
            r->SetUrl(std::string(StrUtils::Split(std::string(sv2.substr(0, second)), '#')[0]));

            std::string_view sv3(line.c_str() + first + 1 + second + 1);
            r->SetProtocol(std::string(sv3.substr(0)));
            return r;
        }();
        size_t methodIndex = request->GetUrl().find('?');
        if (methodIndex != -1) {
            std::vector<std::string_view> &&params = StrUtils::Split({request->GetUrl().c_str() + methodIndex + 1},
                                                                     '&');
            request->SetParams(StrUtils::PropertyParse(params));
        }

        getline(ss, line);
        while (!line.empty() && line[0] != '\r') {
            size_t split = line.find(':');
            std::string key = line.substr(0, split);
            StrUtils::ToLowCase(key);
            request->AddHeader(key, line.substr(split + 1));
            line.clear();
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
    static bool Response(const std::shared_ptr<NetworkAdapter> &client,
                         const std::shared_ptr<HttpRequest> &request,
                         const std::shared_ptr<HttpResponse> &response) {
        if (request->GetHeader().count(CONNECTION) == 1) {
            response->GetHeader()[CONNECTION] = request->GetHeader()[CONNECTION];
        } else {
            response->GetHeader()[CONNECTION] = CLOSE;
        }

        request->GetPartTable().empty() ? ResponseSimple(client, request, response) : ResponseRanges(client, request,
                                                                                                     response);
        return response->GetHeader()[CONNECTION] == KEEP_ALIVE;
    }

    static void ResponseRanges(const std::shared_ptr<NetworkAdapter> &client,
                               const std::shared_ptr<HttpRequest> &request,
                               const std::shared_ptr<HttpResponse> &response) {
        size_t fileLength = response->GetLength();
        if (request->GetPartTable().size() < 2) {
            auto &part = const_cast<std::pair<long long, long long> &>(request->GetPartTable().front());
            if (part.second >= fileLength || part.first >= part.second) {
                response->SetCode(PARTIAL_CONTENT_CODE);
                response->SetLength(0);
            } else {
                size_t sendLength = part.second - part.first;
                std::string contentRange = BYTES;
                contentRange = contentRange + " " + std::to_string(part.first) + "-" +
                               std::to_string(part.second) + "/" +
                               std::to_string(fileLength);
                response->SetLength(sendLength);
                response->GetHeader()[CONTENT_RANGE] = contentRange;
            }

            client->Write(response->GetHeadStream());
            if (request->GetRequestMethod() != RequestMethod::HEAD) {
                client->Write(response->GetFp(), part.first, part.second);
            }
            return;
        }
        ResponseMultipartRanges(client, request, response);
    }

    static void ResponseMultipartRanges(const std::shared_ptr<NetworkAdapter> &client,
                                        const std::shared_ptr<HttpRequest> &request,
                                        const std::shared_ptr<HttpResponse> &response) {
        size_t fileLength = response->GetLength();
        std::string oldContentType = response->GetHeader()[CONTENT_TYPE];
        std::string boundary = "--";
        boundary += StrUtils::GetRandStr(26);
        response->GetHeader()[CONTENT_TYPE] = std::string(MULTIPART_BYTERANGES) + ";boundary=" + boundary;
        response->SetCode(PARTIAL_CONTENT_CODE);

        auto &partTable = const_cast<std::vector<std::pair<long long, long long>> &>(request->GetPartTable());
        std::vector<std::vector<std::pair<std::string, std::string>>> partHeaders;
        std::string contentRange;
        size_t sendLength = 0;
        for (auto &&i : partTable) {
            size_t l = i.second - i.first;
            if (i.second >= fileLength || i.first >= i.second) {
                l = 0;
            }
            sendLength += l;
            contentRange = std::to_string(i.first) + "-" +
                           std::to_string(i.second) + "/" +
                           std::to_string(fileLength);
            partHeaders.emplace_back();
            partHeaders.back().push_back(std::make_pair(CONTENT_TYPE, oldContentType));
            partHeaders.back().push_back(std::make_pair(CONTENT_RANGE, contentRange));
        }
        for_each(partHeaders.begin(), partHeaders.end(), [&](std::vector<std::pair<std::string, std::string>> &i) {
            // 分割换行、空行、数据换行
            sendLength += (boundary + "\r\n\r\n\r\n").size();
            for_each(i.begin(), i.end(), [&](std::pair<std::string, std::string> &j) {
                sendLength += j.first.size() + std::string(":").size() + j.second.size() + std::string("\r\n").size();
            });
        });
        sendLength += (boundary + "--").size();

        response->SetLength(sendLength);
        client->Write(response->GetHeadStream());
        if (request->GetRequestMethod() == RequestMethod::HEAD) {
            return;
        }
        for (size_t i = 0; i < partTable.size(); i++) {
            std::stringstream sH;
            sH << boundary << "\r\n";
            for (auto &&j : partHeaders[i]) {
                sH << j.first << ":" << j.second << "\r\n";
            }
            sH << "\r\n";
            client->Write(sH);
            client->Write(response->GetFp(), partTable[i].first, partTable[i].second);
            sH.str("");
            auto a = sH.str();
            sH << "\r\n";
            client->Write(sH);
        }
        std::stringstream tail;
        tail << boundary << "--";
        client->Write(tail);
    }

    static void ResponseSimple(const std::shared_ptr<NetworkAdapter> &client,
                               const std::shared_ptr<HttpRequest> &request,
                               const std::shared_ptr<HttpResponse> &response) {
        client->Write(response->GetHeadStream());
        if (request->GetRequestMethod() == RequestMethod::HEAD) {
            return;
        }
        if (response->GetFp().is_open()) {
            client->Write(response->GetFp());
        } else {
            client->Write(std::stringstream(response->GetBody()));
        }
    }

public:
    /**
     * @brief 开始执行Http交互
     *
     * @param client 网络适配器
     */
    void AcceptHttp(std::shared_ptr<NetworkAdapter> client) {
        threadPool->AddTask(
                [hi = httpRegisterInterceptor, s = httpRegisterServer, c = client]() {
                    while (true) {
                        auto request = Parse(c);
                        if (nullptr == request) {
                            break;
                        }
                        std::shared_ptr<HttpResponse> response = std::make_shared<HttpResponse>();
                        // 验证前置拦截器
                        if ((hi == nullptr || hi->VerifyBefore(request, response))) {
                            // 映射请求
                            s == nullptr || s->MapRequest(request, response);
                            // 验证后置拦截器
                            if (hi != nullptr && !hi->VerifyAfter(request, response)) {
                                response->SetCode(FORBIDDEN);
                            }
                        } else {
                            response->SetCode(FORBIDDEN);
                        }
                        if (!Response(c, request, response)) {
                            break;
                        }
                    }
                });
    }
};
