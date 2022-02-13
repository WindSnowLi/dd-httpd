#pragma once

#include "ThreadPoolAdapter.hpp"

class NetworkServerThreadPoolAdapter : public ThreadPoolAdapter {
private:

public:
    /**
     * @brief Construct a new Http Thread Pool Adapter object
     *
     * @param maxThread 最大线程数
     */
    explicit NetworkServerThreadPoolAdapter(int maxThread = 20) : ThreadPoolAdapter(maxThread) {};

    /**
     * @brief 添加线程回调任务
     *
     * @param callback 线程回调
     */
    void AddTask(std::function<void()> callback) {
        callback();
    };
};
