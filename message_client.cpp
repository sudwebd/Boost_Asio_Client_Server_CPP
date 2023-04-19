#include "client.h"

class message_client {
    public:
        void on_read(const char* msg, int len) {
            std::string received_msg{msg, static_cast<size_t>(len)};
            std::cout<<"Received from server:\n\t"<<received_msg<<"\n";
        }
};

int main() {
    boost::asio::io_service ios;
    message_client mc;
    client cl(ios, "127.0.0.1", 8080, [&mc](const char* msg, int len){
        mc.on_read(msg, len);
    });
    ios.run();
    return 0;
}