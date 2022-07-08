#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	//Assuming that the server application has
	//already obtained the protocol port number
	unsigned short port_num = 3333;

	//Create special object of asio::ip::address class
	//that specifies all IP-addresses available on the host.
	//Here we assume that the server works over IPv6 protocol
	asio::ip::address ip_address = asio::ip::address_v6::any();

	asio::ip::tcp::endpoint ep(ip_address, port_num);

	//The end point is created and can be used to specify
	//the IP addresses and a port number on which the server
	//application wants to listen for incoming connections.
	return 0;

}