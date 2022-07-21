#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using namespace boost;

//Keep objects we need in a callback to identify whether
//all data has been written to the socket and to initiate
//next async writing operation if needed

struct Session {
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::string buf;
	std::size_t total_bytes_written;
};

//Function used as callback for asynchronous write operation. Checks if all data
//from the buffer has been written to the socket and initiates new asynchronous 
//writing operation if needed. Called when the asynchronous operation is completed


void callback(const system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s)
{
	//When invoked, the callback function begins by checking whether the operation
	//succeeded or an error occured.
	if (ec.value() != 0) {
		std::cout << "Error occured! Error code = " << ec.value() << ". Message: " << ec.message();
		return;
	}

	s->total_bytes_written += bytes_transferred;

	if (s->total_bytes_written == s->buf.length()) {
		return;
	}

	//A new async write operation is initiated.
	s->sock->async_write_some(asio::buffer(s->buf.c_str() + s->total_bytes_written,
		s->buf.length() - s->total_bytes_written),
		std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

void writeToSocket(std::shared_ptr<asio::ip::tcp::socket>sock)
{
	//Since the method may not write all the data to the socket in one go, we may
	//need to initiate another async write operation in the callback function. This
	//is why we nned the Session object and we allocate it in the free memory and not
	//on the stack; it must live until the callback function is called.
	std::shared_ptr<Session> s(new Session);

	//Allocating and filling the buffer.
	s->buf = std::string("Hello");
	s->total_bytes_written = 0;
	s->sock = sock;

	//async_write_some(buffer, callback)
	//Since we want to pass an additional argument to our callback function, a pointer
	//to the corresponfing Session object, which acts as a context for the operation,
	//we use the std::bind() function to construct a function object to which we 
	//attach a pointer to the Session object as the third argument. The function object
	//is then passed as a callback argument to the socket object's async_write_some()
	s->sock->async_write_some(asio::buffer(s->buf),
		std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));

	//Because it is asynchronous, the async_write_some() method doesn't block the thread
	//of execution. It initiates the writing operation and returns.
}



//This application is run by a single thread, in the context of which the application's
//main() entry point function is called. Boost.Asio may create additional threads for
//some internal operations, but it guarantees that no application code is executed in 
//the context of those threads.
int main()
{
	std::string raw_ip_address = "192.168.1.72";
	unsigned short port_num = 3333;

	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

		asio::io_service ios;

		//Allocating and opening the socket
		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

		sock->connect(ep);

		writeToSocket(sock);

		//Capture the thread of execution and use it to call the callback function
		//associated with the asynchronous operations when they get completed.
		//Blocks as long as at least one pending async operation exists, returns when
		//the last pending async operation is completed.
		ios.run();
	}

	catch (system::system_error& e) {
		std::cout << "Error! Error code = " << e.code() << " .Message = " << e.what();
		return e.code().value();
	}

	return 0;
}