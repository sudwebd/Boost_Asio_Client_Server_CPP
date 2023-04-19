#include<iostream>
#include<functional>
#include "utils.h"
#include<boost/asio.hpp>

namespace asio_ip = boost::asio::ip;

class server {
using server_listen_cb = std::function<void(asio_ip::tcp::socket&&)>;
public:
    server(boost::asio::io_service& ios, const std::string ip, int port_num, server_listen_cb&& server_cb) : ios_{ios}, listener_{ios_}, 
    acceptor_{ios_, asio_ip::tcp::endpoint(asio_ip::address::from_string(ip), port_num)}, server_cb_{std::move(server_cb)} {   
        acceptor_.async_accept(listener_, [this](const auto& ec) {
            server_cb_(std::move(listener_));
        });
    }
private:
    // server needs a listener socket and an acceptor for connections
    boost::asio::io_service& ios_;
    asio_ip::tcp::socket listener_;
    asio_ip::tcp::acceptor acceptor_;
    server_listen_cb&& server_cb_;
};
