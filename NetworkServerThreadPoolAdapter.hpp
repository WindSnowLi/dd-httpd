#pragma once

#include <memory>
#include "ThreadPoolAdapter.hpp"
#include "ThreadPool.hpp"

class NetworkServerThreadPoolAdapter : public ThreadPoolAdapter {
private:
    std::shared_ptr<ThreadPool> threadPool;
public:
    /**
     * @brief Construct a new Http Thread Pool Adapter object
     *
     * @param maxThread 最大线程数
     */
    explicit NetworkServerThreadPoolAdapter(int maxThread = 20) : ThreadPoolAdapter(maxThread) {
        threadPool = std::make_shared<ThreadPool>(maxThread);
    };

    /**
     * @brief 添加线程回调任务
     *
     * @param callback 线程回调
     */
    void AddTask(std::function<void()> callback) {
        threadPool->AddTask(callback);
    };
};
