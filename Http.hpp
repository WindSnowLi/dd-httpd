#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>

#include "HttpRegisterServer.hpp"
#include "HttpRegisterInterceptor.hpp"
#include "NetworkRegisterInterceptor.hpp"
#include "NetworkAdapter.hpp"
#include "SocketNetworkAdapter.hpp"
#include "NetworkServer.hpp"
#include "HttpServer.hpp"

/**
 * @brief Http服务
 *
 */
class Http {
protected:
    /**
     * @brief 服务注册器
     *
     */
    std::shared_ptr<HttpRegisterServer> httpRegisterServer;
    /**
     * @brief Http拦截器注册器
     *
     */
    std::shared_ptr<HttpRegisterInterceptor> httpRegisterInterceptor;
    /**
     * @brief 网络拦截器注册器
     *
     */
    std::shared_ptr<NetworkRegisterInterceptor> networkRegisterInterceptor;

    /**
     * @brief 网络适配器
     *
     */
    std::shared_ptr<NetworkAdapter> networkAdapter;
    /**
     * @brief 监听端口
     *
     */
    int port{};
    /**
     * @brief 主要解析服务
     *
     */
    std::shared_ptr<NetworkServer> networkServer;
    /**
     * @brief http服务
     * 
     */
    std::shared_ptr<HttpServer> httpServer;
public:
    /**
     * @brief Construct a new Http object
     *
     * @param httpRegisterServer 使用注册器
     */
    Http(int port, const std::shared_ptr<HttpRegisterServer> &httpRegisterServer,
         std::shared_ptr<NetworkAdapter> netWorkAdapter =
         std::make_shared<SocketNetworkAdapter>()) : port(port),
                                                     httpRegisterServer(httpRegisterServer),
                                                     networkAdapter(std::move(netWorkAdapter)) {
    }

    /**
     * @brief 监听端口
     *
     */
    [[noreturn]] void Listening() {
        if (httpServer == nullptr) {
            httpServer = std::make_shared<HttpServer>();
            httpServer->httpRegisterInterceptor = httpRegisterInterceptor;
            httpServer->httpRegisterServer = httpRegisterServer;
            httpServer->httpServerThreadPoolAdapter = std::make_shared<HttpServerThreadPoolAdapter>();
        }
        if (networkServer == nullptr) {
            networkServer = std::make_shared<NetworkServer>();
            networkServer->networkRegisterInterceptor = networkRegisterInterceptor;
            networkServer->networkServerThreadPoolAdapter = std::make_shared<NetworkServerThreadPoolAdapter>();
            networkServer->httpServer = httpServer;
        }

        networkAdapter->port = port;
        networkAdapter->Listen();
        while (true) {
            std::shared_ptr<NetworkAdapter> client(networkAdapter->Accept());
            networkServer->Accept(client);
        }
    }
};