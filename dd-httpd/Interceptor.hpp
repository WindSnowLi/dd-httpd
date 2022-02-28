#pragma once

#include <string>

class Interceptor {
protected:
	/**
	 * @brief 注册器默认顺序
	 *
	 */
	unsigned int order = -1;
	/**
	 * @brief 注册器唯一ID
	 *
	 */
	std::string id;

public:
	Interceptor(unsigned int order, std::string id) :order(order), id(id) {};

	Interceptor(const Interceptor& interceptor) :order(interceptor.order), id(interceptor.id) {};

	/**
	 * @brief 比较运算符，用于排序拦截器
	 *
	 * @param interceptor 拦截器
	 * @return true 小于
	 * @return false 大于
	 */
	bool operator<(const Interceptor& interceptor) const {
		return this->order < interceptor.order;
	}

	bool operator==(const Interceptor& interceptor) {
		return this->id == interceptor.id;
	}

	bool operator>(const Interceptor& interceptor) const {
		return this->order > interceptor.order;
	}


	virtual ~Interceptor() = default;
};
