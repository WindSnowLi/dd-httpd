#pragma once

#include <memory>

#include "NetworkAdapter.hpp"
#include "Interceptor.hpp"

class NetworkInterceptor : public Interceptor {
public:
    NetworkInterceptor() = default;

    NetworkInterceptor(const NetworkInterceptor &networkInterceptor) = default;

    /**
     * @brief 接受链接执行拦截器
     *
     * @param client 网络适配器
     */
    virtual bool AcceptAfter(std::shared_ptr<NetworkAdapter> client) = 0;
};
