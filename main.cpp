#include "Http.hpp"
#include <iostream>
#include <memory>

int main() {
    auto server = std::make_shared<HttpRegisterServer>();
    server->RegisterGet("/hello", [&](HttpRequest &request, HttpResponse &response) {
        std::cout << "hello" << std::endl;
        response.SetBody(std::string("I LOVE YOU!"));
    });
    Http http(52113, server);
    http.Listening();
    return 0;
}
