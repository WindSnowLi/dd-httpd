#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

#include "HttpInterceptor.hpp"
#include "RegisterInterceptor.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpRegisterInterceptor : public RegisterInterceptor {

public:

    /**
     * @brief 验证Http前置拦截器
     *
     * @param request 请求对象智能指针
     * @param response 应答对象智能指针
     * @return true 通过
     * @return false 拒绝
     */
    bool VerifyBefore(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) {
        for (const Interceptor &interceptor : interceptorSet) {
            if (!((HttpInterceptor *) &(interceptor))->AcceptBefore(request, response)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief 验证Http后置拦截器
     *
     * @param request 请求对象智能指针
     * @param response 应答对象智能指针
     * @return true 通过
     * @return false 拒绝
     */
    bool VerifyAfter(const std::shared_ptr<HttpRequest> &request, const std::shared_ptr<HttpResponse> &response) {
        for (auto &&interceptor : interceptorSet) {
            if (!((HttpInterceptor *) &(interceptor))->AcceptAfter(request, response)) {
                return false;
            }
        }
        return true;
    }
};
