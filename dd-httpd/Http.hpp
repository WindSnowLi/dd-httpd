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
	Http() = default;

	/**
	 * @brief Construct a new Http object
	 *
	 * @param httpRegisterServer 服务注册器
	 */
	Http(int port, std::shared_ptr<HttpRegisterServer> registerServer) : port(port),
		httpRegisterServer(
			std::move(registerServer)) {}

	/**
	 * @brief 监听端口
	 *
	 */
	[[noreturn]] void Listening() {
		httpServer == nullptr ? httpServer = std::make_shared<HttpServer>() : 0;
		httpServer->threadPool == nullptr ? (httpServer->threadPool = std::make_shared<ThreadPool>()) : 0;

		httpRegisterInterceptor = httpServer->httpRegisterInterceptor =
			(httpRegisterInterceptor == nullptr ? std::make_shared<HttpRegisterInterceptor>() : httpRegisterInterceptor);

		httpRegisterInterceptor->Register(std::make_shared<HttpGetFileInterceptor>(1, "HttpGetFileInterceptor"));

		httpRegisterServer = httpServer->httpRegisterServer =
			(httpRegisterServer == nullptr ? std::make_shared<HttpRegisterServer>() : httpRegisterServer);


		networkServer = (networkServer == nullptr ? std::make_shared<NetworkServer>() : networkServer);
		networkServer->threadPool = std::make_shared<ThreadPool>();
		networkRegisterInterceptor = networkServer->networkRegisterInterceptor =
			(networkRegisterInterceptor == nullptr ? std::make_shared<NetworkRegisterInterceptor>() : 0);

		networkServer->httpServer = httpServer;
		networkAdapter == nullptr ? networkAdapter = std::make_shared<SocketNetworkAdapter>() : 0;
		networkAdapter->port = port;
		networkAdapter->Listen();
		while (true) {
			std::shared_ptr<NetworkAdapter> client(networkAdapter->Accept());
			networkServer->Accept(client);
		}
		networkAdapter->Shutdown();
	}

	[[nodiscard]] const std::shared_ptr<HttpRegisterServer>& GetHttpRegisterServer() const {
		return httpRegisterServer;
	}

	void SetHttpRegisterServer(const std::shared_ptr<HttpRegisterServer>& registerServer) {
		Http::httpRegisterServer = registerServer;
	}

	[[nodiscard]] const std::shared_ptr<HttpRegisterInterceptor>& GetHttpRegisterInterceptor() const {
		return httpRegisterInterceptor;
	}

	void SetHttpRegisterInterceptor(const std::shared_ptr<HttpRegisterInterceptor>& registerInterceptor) {
		Http::httpRegisterInterceptor = registerInterceptor;
	}

	[[nodiscard]] const std::shared_ptr<NetworkRegisterInterceptor>& GetNetworkRegisterInterceptor() const {
		return networkRegisterInterceptor;
	}

	void SetNetworkRegisterInterceptor(const std::shared_ptr<NetworkRegisterInterceptor>& registerInterceptor) {
		Http::networkRegisterInterceptor = registerInterceptor;
	}

	[[nodiscard]] const std::shared_ptr<NetworkAdapter>& GetNetworkAdapter() const {
		return networkAdapter;
	}

	void SetNetworkAdapter(const std::shared_ptr<NetworkAdapter>& adapter) {
		Http::networkAdapter = adapter;
	}

	[[nodiscard]] int GetPort() const {
		return port;
	}

	void SetPort1(int p) {
		Http::port = p;
	}

	[[nodiscard]] const std::shared_ptr<NetworkServer>& GetNetworkServer() const {
		return networkServer;
	}

	void SetNetworkServer(const std::shared_ptr<NetworkServer>& server) {
		Http::networkServer = server;
	}
};