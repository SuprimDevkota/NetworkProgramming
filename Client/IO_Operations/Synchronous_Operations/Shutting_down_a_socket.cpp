#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

//Accepts a reference to socket connected to server and performs the communication
//with the server using this socket

void communicate(asio::ip::tcp::socket& sock)
{
	//Allocating and filling the buffer with binary data
	const char request_buf[] = { 0x48, 0x65, 0x0, 0x6c, 0x6c, 0x6f };

	//Sending the request data
	asio::write(sock, asio::buffer(request_buf));

	//Shutting down the socket to let the server know that we've sent the whole request
	//At this point writing to the socket is disabled and there is no way to restore
	//the socket state to make it writable again.
	sock.shutdown(asio::socket_base::shutdown_send);

	//We use extensible buffer for the response because we don't know the size of the
	//response message
	asio::streambuf response_buf;

	system::error_code ec;
	asio::read(sock, response_buf, ec);

	if (ec == asio::error::eof) {
		//Whole response message has been recieved. Here we can handle it.
	}
	
	else
	{
		throw system::system_error(ec);
	}
}

int main()
{
	std::string raw_ip_address = "192.168.1.70";
	unsigned short port_num = 3333;

	try {
		
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		
		asio::io_service ios;
		
		asio::ip::tcp::socket sock(ios, ep.protocol());
		
		sock.connect(ep);
		
		communicate(sock);
	}

	catch (system::system_error& e) {

		std::cout << "Error! Error code = " << e.code() << ". Message: " << e.what();
		
		return e.code().value();
	}
	return 0;
}