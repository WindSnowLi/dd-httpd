#pragma once

#include <functional>

/**
 * @brief 线程池适配器
 *
 */
class ThreadPoolAdapter {
protected:
    /**
     * @brief 最大线程数
     *
     */
    int m_maxThread;

public:
    /**
     * @brief Construct a new Thread Pool Adapter object
     *
     * @param maxThread
     */
    explicit ThreadPoolAdapter(int maxThread) : m_maxThread(maxThread) {};

    /**
     * @brief 设置线程池最大线程数数量
     *
     * @param max 最大线程数量
     */
    void SetMAXThread(int max) {
        this->m_maxThread = max;
    };

    /**
     * @brief Destroy the Thread Pool Adapter object
     * 
     */
    virtual ~ThreadPoolAdapter() = default;
};
