#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main()
{
	//The argument that the listen() method accepts as an argument
	//specifies the size of the queue maintained by the operating system
	//to which it puts connection requests arriving from the clients. 
	//The requests stay in the queue and are waiting for the server
	//application to dequeue and process them. When the queue becomes
	//full, the new connection requests are rejected by the operating system.
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

		//Start listening for incoming connection requests. This call switches
		//the acceptor socket into the state in which it listens for incoming
		//connection requests. Unless listen() is called, all connection
		//requests arriving at the corresponding endpoint will be rejected
		//by the operating system network software.

		
		acceptor.listen(BACKLOG_SIZE);

		//Create an active socket
		asio::ip::tcp::socket sock(ios);

		//Processing the next connection request and connecting the active socket
		//to the client. Accepts an active socket as argument and performs several
		//operations. First, it checks the queue associated with the acceptor socket
		//containind pending connection requests. If the queue is empty, the method
		//blocks execution until a new connection request arrives to an endpoint to
		//which the acceptor socket is bound and the operating system puts in in the queue

		//If at least one connection request is available in the queue, the one on top of
		//the queue is extracted from it and processed. The active socket that was passed to
		//the accept() method as an argument is connected to the corresponding client application
		//which issued the connection request

		//If the connection establishment process succeeds, the accept() method returns
		//and the active socket is opened and connected to the client application and can
		//be used to send data to and recieve data from it.
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