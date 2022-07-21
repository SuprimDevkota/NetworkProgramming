#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

//The precondition is that the socket passed is already connected otherwise the 
//function fails
void writeToSocket(asio::ip::tcp::socket& sock)
{
	//Allocating and filling the buffer
	std::string buf = "Hello World";
	std::size_t total_bytes_written = 0;

	if (buf.length() == 0) {
		std::cout << "Buffer is empty";
		return;
	}
	//Run the loop until all the data is written in the socket
	while (total_bytes_written != buf.length())
	{
		//write_some (start pos of buffer in that iteration, size of buffer in that iteration)
		//write_some() returns the number of bytes written to the socket
		total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + 
			total_bytes_written, buf.length() - total_bytes_written));
	}

	//The above loop can also be simplified as:
	//asio::write(sock, asio::buffer(buf));
}


int main()
{
	const int BACKLOG_SIZE = 5;
	unsigned short port_num = 3333;

	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;

	try
	{
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		acceptor.bind(ep);
		acceptor.listen(BACKLOG_SIZE);
		asio::ip::tcp::socket sock(ios);
		acceptor.accept(sock);
		writeToSocket(sock);
	}

	catch (system::system_error& e) {
		std::cout << "Error! Error code = " << e.code() << " Message = " << e.what();
		return e.code().value();
	}
	return 0;
}