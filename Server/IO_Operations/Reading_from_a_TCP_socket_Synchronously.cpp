#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

//The precondition is that the socket passed is already bound otherwise the 
//function fails
std::string ReadFromSocket(asio::ip::tcp::socket& sock)
{
	//Allocating a buffer for known message size
	const unsigned int MESSAGE_SIZE = 11;
	char buf[MESSAGE_SIZE];
	std::size_t total_bytes_read = 0;
	boost::system::error_code ec;

	while (total_bytes_read != MESSAGE_SIZE)
	{
		total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read,
			MESSAGE_SIZE - total_bytes_read), ec);

		//This condition is required because socket has a read_some() method which
		// returns boost::asio::error::eof on closing
		if (ec == boost::asio::error::eof) {
			break;
		}
	}

	//The above loop can also be simplified as follows and won't need an eof check:
	//asio::read(sock, asio::buffer(buf));
	//return std::string (buf, MESSAGE_SIZE);

	return std::string(buf, total_bytes_read);

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
		asio::ip::tcp::socket sock(ios);
		acceptor.accept(sock);
		std::cout << ReadFromSocket(sock);
	}

	catch (system::system_error& e) {
		std::cout << "Error! Error code = " << e.code() << " Message = " << e.what();
		return e.code().value();
	}
	return 0;
}