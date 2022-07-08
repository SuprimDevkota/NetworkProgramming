#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main()
{
	//The size of the queue containing the pending connection requests.
	const int BACKLOG_SIZE = 30;

	//Assume that the server application has already obtained the protocol port
	//number
	unsigned short port_num = 3333;

	//Creating a server endpoint
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);

	asio::io_service ios;

	try {
		//Instantiating and opening an acceptor socket.
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());

		//Binding the acceptor socket to the server endpoint
		acceptor.bind(ep);

		//Start listening for incoming connection requests.
		acceptor.listen(BACKLOG_SIZE);

		//Create an active socket
		asio::ip::tcp::socket sock(ios);

		//Processing the next connection request and connecting the active socket to the client
		acceptor.accept(sock);

		//At this point sock scoket is connected to the client application
		//and can be used to send data to or recieve data from it.
	}

	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << " .Message: "
			<< e.what();
		
		return e.code().value();
	}
	return 0;
}