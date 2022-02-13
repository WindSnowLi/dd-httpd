#pragma once

#include <vector>
#include <iostream>
#include "NetworkInterceptor.hpp"
#include "RegisterInterceptor.hpp"

class NetworkRegisterInterceptor : public RegisterInterceptor {

public:
    /**
     * @brief 验证拦截器
     *
     * @param client 网络适配器
     * @return true 通过
     * @return false 拒绝
     */
    bool Verify(const std::shared_ptr<NetworkAdapter> &client) {
        for (auto interceptor : interceptorSet) {
            if (!((NetworkInterceptor *) &(interceptor))->AcceptAfter(client)) {
                return false;
            }
        }
        return true;
    }
};
