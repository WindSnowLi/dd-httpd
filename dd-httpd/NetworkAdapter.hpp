#pragma once

#include <memory>
#include <string>
#include <sstream>

class NetworkAdapter {
public:
    /**
     * @brief 监听端口
     *
     */
    int port{};
    /**
     * @brief IP
     *
     */
    unsigned long addr{};

public:
    explicit NetworkAdapter() = default;

    /**
     * @brief 构造
     *
     * @param port 端口号
     * @param addr IP
     */
    explicit NetworkAdapter(int port, unsigned long addr = 0) : port(port), addr(addr) {}

    /**
     * @brief 单字节读
     *
     * @return std::stringstream 读取完整的字符串流
     */
    virtual std::stringstream Read() = 0;

    /**
     * @brief 写入单字节
     *
     * @param str 写入的完整字符串流
     */
    virtual void Write(const std::stringstream &str) = 0;

    /**
     * @brief 开始监听
     *
     */
    virtual void Listen() = 0;

    /**
     * @brief 接受客户端
     *
     * @return NetWorkAdapter* 适配器指针
     */
    virtual NetworkAdapter *Accept() = 0;

    virtual ~NetworkAdapter() = default;
};
