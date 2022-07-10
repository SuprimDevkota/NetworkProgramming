#include <boost/asio.hpp>
#include <iostream>
#include <memory>			//For std::unique_ptr<>
using namespace boost;

int main()
{
	//Expect to recieve a block of data no more than 20 bytes long
	const size_t BUF_SIZE_BYTES = 20;

	//Alocating the buffer
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);

	//Creating buffer representation that satisfies MutableBufferSequence concept
    //mutable_buffers_1 adaptor class represents a single mutable buffer 
	asio::mutable_buffers_1 input_buffer = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);

	//input_buffer is the representation of the buffer 'buf' that can be used in Boost.Asio
	// input operations
	return 0;
}