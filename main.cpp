#include "dd-httpd/Http.hpp"
#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<HttpRegisterServer> server = std::make_shared<HttpRegisterServer>();
    server->RegisterGet("/hello", [&](HttpRequest &request, HttpResponse &response) {
        std::cout << "hello" << std::endl;

        for (auto &&i: request.GetParams()) {
            std::cout << i.first << '\t' << i.second << std::endl;
        }
        response.SetBody(std::string("I LOVE YOU!"));
    });
    server->SetRootPath(std::string("."));
    Http http(52113, server);
    http.Listening();
    return 0;
}
