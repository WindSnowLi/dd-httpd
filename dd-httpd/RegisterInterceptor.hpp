#pragma once

#include <set>
#include <algorithm>
#include <memory>

#include "Interceptor.hpp"

class RegisterInterceptor {
protected:
	std::set<std::shared_ptr<Interceptor>> interceptorSet{};

public:
	/**
	 * @brief 注册拦截器
	 *
	 * @param interceptor 拦截器
	 */
	virtual void Register(const std::shared_ptr<Interceptor>& interceptor) {
		interceptorSet.insert(interceptor);
	}

	virtual ~RegisterInterceptor() = default;
};
