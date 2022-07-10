#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	//Start with instantiating the object of the std::string class. The buffer is allocated and 
	//filled with data.
	const std::string buffer = "Hello";

	//Before an object can be used it must be represented.

	//If the buffer argument passed to the asio::buffer() function is a read-only type,
	//the function returns an object of the asio::const_buffers_1 class; otherwise, an 
	//object of the asio::mutable_buffers_1 class is returned.

	//const_buffers_1 adaptor class represents a single constant buffer
	asio::const_buffers_1 output_buffer = asio::buffer(buffer);

	//output_buffer is the representation of the buffer 'buffer' that can be used in
	//Boost.Asio output operations.
	return 0;
}