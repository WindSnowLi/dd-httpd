#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <tuple>

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


    char buff[2048]{0};

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
    virtual std::tuple<std::stringstream, bool> Read() = 0;

    /**
     * @brief 写入字符串流对象
     *
     * @param str 写入的完整字符串流
     */
    virtual void Write(const std::stringstream &str) = 0;

    /**
     * @brief 写入文件
     *
     * @param fp 文件
     */
    virtual void Write(std::ifstream &fp) = 0;

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
