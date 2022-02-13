#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

#include "NetworkRegisterInterceptor.hpp"
#include "NetworkServerThreadPoolAdapter.hpp"
#include "HttpServer.hpp"

#ifdef _WIN32

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#endif

class NetworkServer {
public:
    /**
     * @brief 拦截器注册器
     *
     */
    std::shared_ptr<NetworkRegisterInterceptor> networkRegisterInterceptor;
    /**
     * @brief 监听线程适配器
     *
     */
    std::shared_ptr<NetworkServerThreadPoolAdapter> networkServerThreadPoolAdapter;
    /**
     * @brief Http数据解析服务
     *
     */
    std::shared_ptr<HttpServer> httpServer;


public:
    /**
     * @brief 接受链接后开始识别
     *
     * @param client 网络适配器
     */
    void Accept(std::shared_ptr<NetworkAdapter> client) {
        this->networkServerThreadPoolAdapter->AddTask(
                [s = networkRegisterInterceptor, h = httpServer, c = client]() {
                    if (s != nullptr && !s->Verify(c)) {
                        return;
                    }
                    h->AcceptHttp(c);
                });
    }
};
