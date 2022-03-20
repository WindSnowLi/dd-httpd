#include "dd-httpd/Http.hpp"
#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<HttpRegisterServer> server = std::make_shared<HttpRegisterServer>();
    server->RegisterGet("/hello", [&](HttpRequest &request, HttpResponse &response) {
        std::cout << "Hello GET!" << std::endl;

        for (auto &&i: request.GetParams()) {
            std::cout << i.first << '\t' << i.second << std::endl;
        }
        response.SetBody(std::string("I LOVE YOU!"));
    });
    server->RegisterPost("/hello", [&](HttpRequest &request, HttpResponse &response) {
        std::cout << "Hello POST!" << std::endl;
        std::cout << request.GetBody() << std::endl;
        for (auto &&i: request.GetParams()) {
            std::cout << i.first << '\t' << i.second << std::endl;
        }
        response.SetBody(std::string("ME TOO!"));
    });
    server->RegisterHead("/hello", [&](HttpRequest &request, HttpResponse &response) {
        std::cout << "Hello HEAD!" << std::endl;
        std::cout << request.GetBody() << std::endl;
        for (auto &&i: request.GetParams()) {
            std::cout << i.first << '\t' << i.second << std::endl;
        }
        response.SetBody(std::string("I THINK...!"));
    });
    server->SetRootPath(std::string("D:\\迅雷下载"));
    server->EnableFileServer();
    Http http(52113, server);
    http.Listening();
    return 0;
}
