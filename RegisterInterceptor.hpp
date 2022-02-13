#pragma once

#include <set>
#include <algorithm>

#include "Interceptor.hpp"

class RegisterInterceptor {
protected:
    std::set<Interceptor> interceptorSet{};

public:
    /**
     * @brief 注册拦截器
     *
     * @param interceptor 拦截器
     */
    void Register(const Interceptor &interceptor) {
        interceptorSet.insert(interceptor);
    }


    virtual ~RegisterInterceptor() = default;
};
