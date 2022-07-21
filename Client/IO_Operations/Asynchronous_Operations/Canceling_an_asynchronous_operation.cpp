#include <boost/predef.h>	//Tools to identify the OS
#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if WIN32_WINNT <= 0x0502 //Windows Server 20003 or earlier
#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO
#endif
#endif

#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace boost;

int main()
{
	std::string raw_ip_address = "192.168.1.70";
	unsigned short port_num = 3333;

	try {
		
		//Allocating and opening a TCP socket

		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

		asio::io_service ios;

		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

		//The callback provided to the async connect is implemented as a lambda function
		sock->async_connect(ep, [sock](const boost::system::error_code& ec)
			{
				//If asynchrnous operation has been cancelled or an error
				//occured during execution, ec contains corresponding error code.

				if (ec.value() != 0) {
					if (ec == asio::error::operation_aborted) {
						std::cout << "Operation cancelled! ";
					}

					else {
						std::cout << "Error! Error code = " << ec.value() << " Message: "
							<< ec.message();
					}

					return;
				}

				//At this point the socket is connected and can be used for communication
				//with remote application
			});

		//Spawning a thread which will be used to call the callback when async operation
		//completes
		std::thread worker_thread([&ios]() {
			try {
				ios.run();
			}

			catch (system::system_error& e) {
				std::cout << "Error! Error code = " << e.code() << " Message: "
					<< e.what();
			}
		});

		//Emulating delay and making the main thread sleep for 2 seconds
		std::this_thread::sleep_for(std::chrono::seconds(2));

		//Canceling the initiated operation. If the operation has not yet been finished
		//it will be canceled and the corresponding callback will be invoked with an
		//argument that specified the error code. If the operation has been completed
		//the cancel() has no effect.
		sock->cancel();

		//Waiting for the worker thread to join
		worker_thread.join();
	}

	catch (system::system_error& e)
	{
		std::cout << "Error! Error code = " << e.code() << " Message: " << e.what();
		return e.code().value();
	}
	return 0;
}