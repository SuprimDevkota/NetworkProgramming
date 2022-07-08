#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main()
{
	//Assume that the server application has already obtained the protocol port number
	unsigned short port_num = 3333;

	//Create an endpoint
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);

	asio::io_service ios;

	//The endpoint created contains information about the transport protocol and the
	//version of the underlying IP protocol
	asio::ip::tcp::acceptor acceptor(ios, ep.protocol());

	boost::system::error_code ec;

	//Binding the acceptor socket
	acceptor.bind(ep, ec);

	if (ec.value() != 0) {
		//Failed to open acceptor socket
		std::cout << "Failed to bind the acceptor socket!" << "Error code = "
			<< ec.value() << ". Message: " << ec.message();
		return ec.value();
	}
	return 0;
}