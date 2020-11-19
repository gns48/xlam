#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

namespace ba = boost::asio;
namespace bi = boost::asio::ip;
namespace bs = boost::system;

class CEechoSession: public std::enable_shared_from_this<CEechoSession> {
private:
    static size_t constexpr max_length = 1024;
    bi::tcp::socket m_socket;
    char m_data[max_length];

    void do_read() {
        auto self(shared_from_this());  // must exist until async op in progress
        m_socket.async_read_some(ba::buffer(m_data, max_length),
                                 [this, self](bs::error_code ec, size_t length) {
                                     if (!ec) do_write(length);
                                 });
    }

    void do_write(size_t length) {
        auto self(shared_from_this()); // must exist until async op in progress
        ba::async_write(m_socket, ba::buffer(m_data, length),
                           [this, self](bs::error_code ec, size_t /*length*/) {
                               if (!ec) do_read();
                           });
    }
    
public:
    CEechoSession(const CEechoSession&) = delete;
    CEechoSession& operator = (const CEechoSession&) = delete;
    
    explicit CEechoSession(bi::tcp::socket&& s) : m_socket(std::move(s)) {
        std::cout << boost::format("Session opened: %d\n") % m_socket.native_handle();
    };
    
    ~CEechoSession() {
        std::cout << boost::format("Session closed: %d\n") % m_socket.native_handle();
    }
    
    inline void start() {
        // we must call do_read() after constructor completion
        do_read();
    }    
};

class CEchoServer {
private:
    ba::io_context& m_io_context;
    bi::tcp::acceptor m_acceptor;    

    void do_accept() {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, bi::tcp::socket socket) {
                if (!ec) std::make_shared<CEechoSession>(std::move(socket))->start();
                do_accept();
            });
    }
    
public:
    CEchoServer(const CEchoServer&) = delete;
    CEchoServer& operator = (const CEchoServer&) = delete;
    
    CEchoServer(ba::io_context& io_context, const bi::tcp::endpoint& endpoint)
        : m_io_context(io_context), m_acceptor(io_context, endpoint)
    {
        do_accept();
    }

};

int main(int ac, char **av) {
    if(ac < 2) {
        std::cerr << boost::format("Usage: %s <listening port>\n") % av[0];
        return EINVAL;
    }

    try {
        ba::io_context io_context;
        unsigned short port = boost::lexical_cast<unsigned short>(av[1]);
        bi::tcp::endpoint endpoint(bi::tcp::v4(), port);
        CEchoServer s(io_context, endpoint);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << boost::format("Exception: %s\n") % e.what();
        return EFAULT;
    }
    
    return 0;
}


    
    


