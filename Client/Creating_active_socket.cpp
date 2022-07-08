#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	//An instance of io_service class is required by the socket constructor.
	//io_service class provides access to network I/O services of the underlying
	//operating system. Sockets get access to those services through the objects of
	//this class so all socket class constructors require this object
	asio::io_service ios;

	//Create an object of tcp class representing a TCP protocol with IPv4 as
	//underlying protocol. Acts like a data structure that contains a set of values
	//that describe the protocol. Doesn't have a public constructor instead has
	//two static methods v4() and v6()
	asio::ip::tcp protocol = asio::ip::tcp::v4();

	//Instantiate an active TCP socket object.
	asio::ip::tcp::socket sock(ios);

	//Used to store error info
	boost::system::error_code ec;

	//Opening the socket. The operating system's socket is allocated in this step
	//Opeening a socket means associating it with a full set of parameters describing
	//a specific protocol over which the socket is intended to be communicating.
	sock.open(protocol, ec);

	if (ec.value() != 0) {
		//Failed to open socket
		std::cout << "Failed to open socket! Error code = " << ec.value()
			<< " Message = " << ec.message();
		return ec.value();
	}

	return 0;
}