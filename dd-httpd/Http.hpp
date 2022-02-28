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
#include "HttpGetFileInterceptor.hpp"

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
	Http(int port,
		std::shared_ptr<HttpRegisterServer> httpRegisterServer,
		std::shared_ptr<NetworkAdapter> netWorkAdapter =
		std::make_shared<SocketNetworkAdapter>(),
		std::shared_ptr<HttpRegisterInterceptor> httpRegisterInterceptor =
		std::make_shared<HttpRegisterInterceptor>()) :
		port(port),
		httpRegisterServer(std::move(httpRegisterServer)),
		networkAdapter(std::move(netWorkAdapter)),
		httpRegisterInterceptor(std::move(httpRegisterInterceptor)) {
	}

	/**
	 * @brief 监听端口
	 *
	 */
	[[noreturn]] void Listening() {
		if (httpServer == nullptr) {
			httpServer = std::make_shared<HttpServer>();
			httpServer->httpRegisterInterceptor = httpRegisterInterceptor;
			httpServer->httpRegisterInterceptor->Register(
				std::make_shared<HttpGetFileInterceptor>(1, "HttpGetFileInterceptor"));
			httpServer->httpRegisterServer = httpRegisterServer;
			httpServer->threadPool = std::make_shared<ThreadPool>();
		}
		if (networkServer == nullptr) {
			networkServer = std::make_shared<NetworkServer>();
			networkServer->networkRegisterInterceptor = networkRegisterInterceptor;
			networkServer->threadPool = std::make_shared<ThreadPool>();
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