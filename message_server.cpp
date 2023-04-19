#include "server.h"
#include <memory>

class message_server {
public:
message_server() {};
void handle_message(boost::asio::ip::tcp::socket&& sock) {
    std::cout<<"Received connect from client\n";
    sock_ = std::make_unique<utils::socket_with_cb>(sock, [this](const char* buf, int l) {
        process_message(buf, l);
    });
    const char* test_msg = "Hello this is server you are connected\n";
    sock_->send_message(test_msg, strlen(test_msg));
}
void process_message(const char* msg, int len) {
    std::cout<<"processing message, read "<<len<<" bytes\n";
}
private:
    std::unique_ptr<utils::socket_with_cb> sock_;
};
int main() {
    boost::asio::io_service ios;
    message_server ms;
    server sv{ios, "127.0.0.1", 8080, [&ms](auto&& sock) {
        ms.handle_message(std::move(sock));
    }};
    ios.run();
    return 0;
}