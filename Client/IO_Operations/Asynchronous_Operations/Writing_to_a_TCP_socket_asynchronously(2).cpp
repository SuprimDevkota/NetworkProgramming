#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost;

struct Session {
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::string buf;
};

void callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s)
{

	if (ec.value() != 0) {
		std::cout << "Error! Error code = " << ec.value() << " .Message = " << ec.message();
		return;
	}
}

void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
{
	std::shared_ptr<Session> s(new Session);

	s->sock = sock;
	s->buf = std::string("Hello");

	asio::async_write(*(s->sock), asio::buffer(s->buf), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main()
{
	std::string raw_ip_address = "192.168.1.72";
	unsigned short port_num = 3333;

	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		
		asio::io_service ios;

		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

		sock->connect(ep);

		writeToSocket(sock);

		ios.run();
	}

	catch (system::system_error& e){
		std::cout << "Error! Error code = " << e.code() << " .Message = " << e.what();
		return e.code().value();
	}

	return 0;
}