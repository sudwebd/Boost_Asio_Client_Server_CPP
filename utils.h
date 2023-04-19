#include <boost/asio.hpp>

namespace asio_ip = boost::asio::ip;
namespace utils {
using on_read_cb = std::function<void(const char *, int)>;
struct socket_with_cb {
    socket_with_cb(asio_ip::tcp::socket &socket, on_read_cb &&cb)
        : socket_{socket}, cb_(std::move(cb)) {
            if(!socket_.is_open()) socket_.open(boost::asio::ip::tcp::v4());
            if(!set_socket_options(true /* keep_alive */)) {
                std::cout<<"Failed to set socket option\n";
                abort();
            }
        }
    void start_listener() {
        socket_.async_receive(boost::asio::buffer(read_buffer_), [this](const auto& ec, auto bytes_read) {
            std::cout<<"sock:read "<<bytes_read<<" bytes\n";
            cb_(read_buffer_, bytes_read);
        });
    }
    auto send_message(const char* msg, int len) {
        std::cout<<"sock:sending "<<len<<" bytes\n";
        socket_.async_send(boost::asio::buffer(msg, len), [](const auto& ec, auto bytes_read) {});
    }
    private:
    template<typename... Options>
    bool set_socket_options(Options... opt) {
        return (*this)(std::forward<Options>(opt)...);
    }    
    // boost::asio::socket_base constains the various socket options
    bool operator()(bool keep_alive) {
        boost::system::error_code ec;
        socket_.set_option(boost::asio::socket_base::keep_alive(true), ec);
        return ec ? false : true;
    }
    asio_ip::tcp::socket &socket_;
    on_read_cb &&cb_;
    static constexpr size_t max_buff_size{1024};
    char read_buffer_[max_buff_size];
};

} // namespace utils