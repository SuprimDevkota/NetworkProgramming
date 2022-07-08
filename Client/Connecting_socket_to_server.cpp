#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		//Create endpoint designating a target server application
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_service ios;

		asio::ip::tcp::socket sock(ios, ep.protocol());

		//This function connects the socket to the server. The connection is performed
		//synchronously, which means that the method blocks the caller thread until
		//either the connection operation is established or an error occurs. The socket
		//is bound to the endpoint consisting of an IP address and a protocol port 
		//number chosen by the operating system
		sock.connect(ep);

		//At this point socket sock is connected to the server application and can
		//be used to send data to or recieve data from it. 
	}
	//Overloads of asio::ip::address::from_string() and asio::ip::tcp::socket::connect()
	//used here throw exceptions in case of error condition.
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << " Message: "
			<< e.what();

		return e.code().value();
	}

	return 0;
}