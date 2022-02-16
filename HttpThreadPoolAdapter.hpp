#pragma once

#include <memory>

#include "ThreadPoolAdapter.hpp"

#include "ThreadPool.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"


class HttpThreadPoolAdapter : public ThreadPoolAdapter {
private:
    /**
     * @brief 线程池
     *
     */
    std::shared_ptr<ThreadPool> threadPool;

public:
    /**
     * @brief Construct a new Http Thread Pool Adapter object
     *
     * @param maxThread 最大线程数
     */
    explicit HttpThreadPoolAdapter(int maxThread = 20) : ThreadPoolAdapter(maxThread) {
        threadPool = std::make_shared<ThreadPool>(maxThread);
    };

    /**
     * @brief 添加线程回调任务
     *
     * @param callback 线程回调
     * @param request 请求对象
     * @param response 应答对象
     */
    void AddTask(std::function<void(HttpRequest, HttpResponse)> callback, HttpRequest request, HttpResponse response) {
        threadPool->AddTask(callback, request, response);
    };
};
