#include <iostream>
#include <asio.hpp>


int main()
{
	asio::io_context io_context;

	asio::ip::tcp::socket socket(io_context);

	std::string ip = "127.0.0.1";//local host
	socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), 8080));


	std::thread t([&]
		{
			while (true)
			{
				asio::error_code ec;
				std::array<char, 128> buf;
				size_t bytes = socket.read_some(asio::buffer(buf), ec);
				if (ec)
				{
					std::cout << ec.message() << '\n';
					break;
				}
				std::cout << std::string(buf.data(), bytes);
			}
		});
	while (true)
	{
		asio::error_code ec;
		std::string message;

		std::getline(std::cin, message);
		socket.write_some(asio::buffer(message.data(), message.length()), ec);

	}
	t.join();

}

