#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	//Assume that the client application has already obtained the DNS name, protocol
	//port number and represented them as strings
	std::string host_name = "samplehost.com";
	std::string port_num = "3333";

	//Used by resolver and socket
	asio::io_service ios;

	//Creating a resolver's query
	asio::ip::tcp::resolver::query resolver_query(host_name, port_num, asio::ip::tcp::resolver::query::numeric_service);

	//Creating a resolver
	asio::ip::tcp::resolver resolver(ios);


	try {
		//Resolving a DNS name
		asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);

		//Creating a socket
		asio::ip::tcp::socket sock(ios);

		//Iterates over each endpoint until successfully connects to one of them.
		//It will throw and exception if it fails to connect to all the endpoints
		//or if other error occurs. asio::connect() opens the socket before connecting
        //it to each endpoint specifying proper protocol object and closes it if the
        //connection fails
		asio::connect(sock, it);

		//At this point socket sock is connected to the server application and
		//can be used to send data to or recieve data from it.
	}

	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message = "
			<< e.what();

		return e.code().value();
	}
	return 0;
}