#include <iostream>
#include <string>
#include <functional>
#include "utils.h"
#include <boost/asio.hpp>

namespace asio_ip = boost::asio::ip;

class client{
using read_cb = std::function<void(const char*, int)>;
public:
    template<typename... Options>
    client(boost::asio::io_service& ios, const std::string& ip, uint16_t port_num, read_cb&& cb, Options... options) : socket_{ios},
    sock_{socket_, std::move(cb)} {
        asio_ip::tcp::endpoint ep{asio_ip::address::from_string(ip), port_num};
        socket_.async_connect(ep, [this](const auto& ec) {
            sock_.start_listener();
        });
    }
    void send_message(const char* buf, int len) {
        sock_.send_message(buf, len);
    }
private:
    asio_ip::tcp::socket socket_;
    utils::socket_with_cb sock_;
};