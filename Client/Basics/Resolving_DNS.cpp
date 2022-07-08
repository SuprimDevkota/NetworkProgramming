#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	//Assume that the client application has already obtained the DNS name
	//and protocol port number and represented them as strings
	std::string host = "samplehost.com";
	std::string port_num = "3333";

	asio::io_service ios;

	//Creating a query for the DNS, contains the DNS name to resolve, a port number
	//that will be used to construct an endpoint object after the DNS name resolution
	//and a set of flags controlling some aspects of the resolution process, represented
	//as a bitmap. In this case the flag informs the query object that the service is 
	//a protocol port number and not a service name
	asio::ip::tcp::resolver::query resolver_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);

	//Creating a resolver. Provides the DNS name resolution functionality
	asio::ip::tcp::resolver resolver(ios);

	//Store error information
	boost::system::error_code ec;

	//DNS name resolution. If succesfull it returns an iterator pointing to the first
	//element of a collection represnenting resolution results. The collection contains
	//object of the asio::ip::basic_resolver_entry<tcp> class. There are as many objects
	//in the collection as the total number of ip addresses that the resolution yielded.
	//Each collection element contains an object of the asio::ip::tcp::endpoint class
	//instantiated from one IP address resulting from the resolution process and a port
	//number provided by the corresponding query object. The endpoint object can be 
	//accessed through the asio::ip::basic_resolver_entry<tcp>::endopoint() getter method.
	
	//The resulting collection may contain endpoints representing both the v4 and v6 addresses
	asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query, ec);


	// Handling errors if any.
	if (ec.value() != 0) {
		// Failed to resolve the DNS name. Breaking execution.
		std::cout << "Failed to resolve a DNS name."
			<< "Error code = " << ec.value()
			<< ". Message = " << ec.message();
		return ec.value();
	}

	//Sample demonstrating how we can iterate through the elements of the collection
	//representing the DNS name resolution process results and how to access the
	//resulting endpoint objects.
	asio::ip::tcp::resolver::iterator it_end;
	for (; it != it_end; ++it) {
		asio::ip::tcp::endpoint ep = it->endpoint();
	}
	
	//Usually, when a DNS name of the host running the server application is resolved
	//to more than one IP addressand correspondingly to more than one endpoint, the
	//client application doesn't know which one of the multiple endpoints to prefer.
	//The common approach in this case is to try to communicate with each endpoint 
	//one by one, until the desired response is received.

	return 0;
}