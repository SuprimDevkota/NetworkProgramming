#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost;

struct Session
{
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::unique_ptr<char[]> buf;
	unsigned int buf_size;
};

void callback(const boost::system::error_code& ec, std::size_t butes_transferred, std::shared_ptr<Session> s)
{
	if (ec.value() != 0)
	{
		std::cout << "Error! Error code = " << ec.value() << " .Message = " << ec.message();
		return;
	}

	std::cout << std::string(s->buf.get(), s->buf_size);
}

void ReadFromSocket(std::shared_ptr < asio::ip::tcp::socket> sock)
{
	std::shared_ptr<Session> s(new Session);
	const int MESSAGE_SIZE = 5;
	s->sock = sock;
	s->buf.reset(new char[MESSAGE_SIZE]);
	s->buf_size = MESSAGE_SIZE;
	asio::async_read(*(s->sock), asio::buffer(s->buf.get(), s->buf_size), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

int main()
{
	const int BACKLOG_SIZE = 5;
	unsigned short port_num = 3333;

	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;

	try
	{
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		acceptor.bind(ep);
		acceptor.listen(BACKLOG_SIZE);
		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios));
		acceptor.accept(*sock);
		ReadFromSocket(sock);
		ios.run();
	}

	catch (system::system_error& e)
	{
		std::cout << "Error! Error code = " << e.code() << " Message = " << e.what();
		return e.code().value();
	}

	return 0;
}