#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main()
{
	//An instance of 'io_service' class is required by
	//the socket constructor. This class is needed by all the Boost.Asio components
	//that need access to the services of the underlying operating system.
	asio::io_service ios;

	//Creating an object of 'tcp' class representing
	// a TCP protocol with IPv6 as underlying protocol.
	asio::ip::tcp protocol = asio::ip::tcp::v6();

	//Instantiating an acceptor socket object.
	asio::ip::tcp::acceptor accept(ios);

	// Used to store information about error that happens
	// while opening the acceptor socket.
	boost::system::error_code ec;

	//Opening the acceptor socket.
	accept.open(protocol, ec);

	if (ec.value() != 0) {
		//Failed to open acceptor socket
		std::cout << "Failed to open the acceptor socket!" << "Error code = "
			<< ec.value() << ". Message: " << ec.message();
		return ec.value();
	}
	return 0;
}