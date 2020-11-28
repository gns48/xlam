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

int main(int ac, char **av) {
    static constexpr size_t max_length = 1024;
    char line[max_length];
    
    if(ac < 3) {
        std::cerr << boost::format("Usage: %s <host> <port>\n") % av[0];
        return EINVAL;
    }

    try {
        ba::io_context io_context;
        bi::tcp::socket s(io_context);
        bi::tcp::resolver resolver(io_context);
        ba::connect(s, resolver.resolve(av[1], av[2]));
        
        while (std::cin.getline(line, max_length)) {
            size_t reqlen = strlen(line);
            ba::write(s, ba::buffer(line, reqlen));
            size_t replen = boost::asio::read(s, ba::buffer(line, reqlen));
            std::cout.write(line, replen); std::cout << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << boost::format("Error: %s\n") % e.what();
        return EFAULT;
    }
    
    return 0;
}
