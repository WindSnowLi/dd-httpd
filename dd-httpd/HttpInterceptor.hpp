#pragma once

#include "Interceptor.hpp"
#include <memory>
#include <utility>

class HttpInterceptor : public Interceptor {
public:

	HttpInterceptor(unsigned int order, std::string id) :Interceptor(order, std::move(id)) {};

	HttpInterceptor(const HttpInterceptor& interceptor) = default;

	/**
	 * @brief Http前置拦截器
	 *
	 * @param request 请求对象智能指针
	 * @param response 应答对象智能指针
	 * @return true 通过
	 * @return false 拒绝
	 */
	virtual bool AcceptBefore(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) = 0;

	/**
	 * @brief Http后置拦截器
	 *
	 * @param request 请求对象智能指针
	 * @param response 应答对象智能指针
	 * @return true 通过
	 * @return false 拒绝
	 */
	virtual bool AcceptAfter(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) = 0;
};
