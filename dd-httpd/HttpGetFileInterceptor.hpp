#pragma once

#include <utility>

#include "HttpInterceptor.hpp"

class HttpGetFileInterceptor : public HttpInterceptor {
public:
	HttpGetFileInterceptor(unsigned int order, std::string id) :HttpInterceptor(order, std::move(id)) {};

	HttpGetFileInterceptor(const HttpGetFileInterceptor& interceptor) = default;
	/**
	 * @brief Http前置拦截器
	 *
	 * @param request 请求对象智能指针
	 * @param response 应答对象智能指针
	 * @return true 通过
	 * @return false 拒绝
	 */
	bool AcceptBefore(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) override {
		return true;
	}

	/**
	 * @brief Http后置拦截器
	 *
	 * @param request 请求对象智能指针
	 * @param response 应答对象智能指针
	 * @return true 通过
	 * @return false 拒绝
	 */
	bool AcceptAfter(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) override {
		if (!request->GetFilePath().empty()) {
			auto sysPath = std::filesystem::u8path(request->GetFilePath());
			if (std::filesystem::exists(sysPath) &&
				StrUtils::CheckPathDeepin(request->GetFilePath())) {
				if (std::filesystem::is_directory(sysPath)) {
					request->GetFilePath().append("/index.html");
				}
				response->SetFp(request->GetFilePath());
			}
			else {
				response->SetCode(NOT_FOUNT);
			}
		}
		return true;
	}

};