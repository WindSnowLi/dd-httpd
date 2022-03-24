#pragma once

#include <sstream>
#include <fstream>
#include <tuple>
#include <thread>

#include "NetworkAdapter.hpp"

#ifdef _WIN32

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#endif

class SocketNetworkAdapter : public NetworkAdapter {
public:
    /**
     * @brief 监听端口的socket
     *
     */
    unsigned long long socket{};

    /**
     * @brief Construct a new Socket Net Work Adapter object
     * @param ip IP
     * @param port 监听端口
     * @param socket 客户端socket
     */
    SocketNetworkAdapter(unsigned long ip, int port, unsigned long long socket) {
        this->addr = ip;
        this->port = port;
        this->socket = socket;
    }

public:
    /**
     * @brief Construct a new Socket Net Work Adapter object
     *
     */
    explicit SocketNetworkAdapter() = default;

    /**
     * @brief Construct a new Socket Net Work Adapter object
     *
     * @param port 监听端口
     */
    explicit SocketNetworkAdapter(int port) {
        this->port = port;
    }

    /**
     * @brief Destroy the Socket Net Work Adapter object
     *
     */
    ~SocketNetworkAdapter() override {
        closesocket(this->socket);
    }

    /**
     * @brief 单字节读
     *
     * @return std::stringstream 读取完整的字符串流
     */
    std::tuple<std::stringstream, bool> Read() override {
        int len;
        std::stringstream ss{};
        while ((len = recv(this->socket, buff, sizeof buff, 0)) != 0) {
            if (len == SOCKET_ERROR) {
                return std::tuple<std::stringstream, int>(std::move(ss), false);
            }
            ss << buff;
            // 剩余
            unsigned long bytesToRecv;
            if (ioctlsocket(this->socket, FIONREAD, &bytesToRecv) == 0) {
                break;
            }
            memset(buff, 0, sizeof buff);
        }
        return std::tuple<std::stringstream, int>(std::move(ss), true);
    }

    /**
     * @brief 写入单字节
     *
     * @param str 写入的完整字符串流
     */
    void Write(const std::stringstream &str) override {
        send(this->socket, str.str().c_str(), (int) str.str().length(), 0);
    }

    void Write(std::ifstream &fp) override {
        if (!fp) {
            return;
        }
        while (true) {
            fp.read(buff, sizeof buff);
            if (fp.gcount() == 0) {
                break;
            }
            std::this_thread::yield();
            auto sd = send(this->socket, buff, (int) fp.gcount(), 0);
            if (SOCKET_ERROR == sd) {
                auto err = WSAGetLastError();
                if (10053 == err || 10054 == err) {
                    break;
                }
            } else if (0 == sd) {
                break;
            }
        }
    }

    void Write(std::ifstream &fp, long long start, long long end) override {
        if (!fp || (start >= end)) return;
        fp.seekg(start, std::ifstream::beg);
        while (start < end) {
            long long readLength = (start + MAX_BUFF <= end ? MAX_BUFF : (end - start));
            fp.read(buff, readLength);
            long long rLen = fp.gcount();
            if (rLen == 0) {
                break;
            }
            auto sd = send(this->socket, buff, (int) rLen, 0);
            if (SOCKET_ERROR == sd) {
                auto err = WSAGetLastError();
                if (10053 == err || 10054 == err) {
                    break;
                }
            } else if (0 == sd) {
                break;
            }
            start += rLen;
        }
    }

    /**
     * @brief 开始监听
     *
     */
    void Listen() override {
        unsigned short sockVersion = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(sockVersion, &wsaData) != 0) {
            throw "WSAStartup Error";
        }

        if ((socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
            throw "create socket error !";
        }

        //绑定IP和端口
        sockaddr_in sin{};
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.S_un.S_addr = INADDR_ANY;
        if (::bind(socket, (LPSOCKADDR) &sin, sizeof(sin)) == SOCKET_ERROR) {
            throw "bind error !";
        }

        //开始监听
        if (listen(socket, 20) == SOCKET_ERROR) {
            throw "listen error !";
        }
    }

    /**
     * @brief 接受客户端
     *
     * @return SocketNetworkAdapter* 适配器指针
     */
    SocketNetworkAdapter *Accept() override {
        //循环监听
        SOCKET sClient;
        sockaddr_in remoteIp{};
        int ipLen = sizeof(remoteIp);
        sClient = accept(socket, (SOCKADDR *) &remoteIp, &ipLen);
        if (sClient == INVALID_SOCKET) {
            return nullptr;
        }
        return new SocketNetworkAdapter(remoteIp.sin_addr.S_un.S_addr, remoteIp.sin_port, sClient);
    }

    void Shutdown() override {
        closesocket(socket);
    }

    static fd_set &getFS() {
        static fd_set set;
        return set;
    }

    static void AddFD(unsigned long long &fd) {
        FD_SET(fd, &getFS());
    }

    static void ZeroFD() {
        FD_ZERO(&getFS());
    }

    static void ClrFD(unsigned long long &fd) {
        FD_CLR(fd, &getFS());
    }

    static bool IsSet(unsigned long long &fd) {
        return FD_ISSET(fd, &getFS());
    }
};
