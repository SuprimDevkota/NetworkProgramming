#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost;

struct Session {
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::unique_ptr<char[]> buf;
	unsigned int buf_size;
};

void callback(const system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s)
{
	if (ec.value() != 0) {
		std::cout << "Error! Error code = " << ec.value() << " .Message = " << ec.message();
		return;
	}
	std::cout << std::string(s->buf.get(), s->buf_size);
}

void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
{
	std::shared_ptr<Session> s(new Session);

	//Allocating a buffer for known message size
	const int MESSAGE_SIZE = 5;
	s->sock = sock;
	s->buf.reset(new char[MESSAGE_SIZE]);
	s->buf_size = MESSAGE_SIZE;
	asio::async_read(*(s->sock), asio::buffer(s->buf.get(), s->buf_size), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main()
{
	std::string raw_ip_address = "192.168.1.70";
	unsigned short port_num = 3333;

	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

		asio::io_service ios;

		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

		sock->connect(ep);

		readFromSocket(sock);

		ios.run();
	}

	catch (std::system_error& e) {
		std::cout << "Error! Error code = " << e.code() << " .Message = " << e.what();
		return e.code().value();
	}

	return 0;
}