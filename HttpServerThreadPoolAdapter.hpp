#pragma once

#include <functional>
#include <memory>

#include "ThreadPoolAdapter.hpp"


class HttpServerThreadPoolAdapter : public ThreadPoolAdapter {
private:

public:
    /**
     * @brief Construct a new Http Listen Thread Pool Adapter object
     *
     * @param maxThread 最大线程数
     */
    explicit HttpServerThreadPoolAdapter(int maxThread = 20) : ThreadPoolAdapter(maxThread) {};

    /**
     * @brief 添加线程回调任务
     *
     * @param callback 线程回调
     */
    void AddTask(std::function<void()> callback) {
        callback();
    };
};
