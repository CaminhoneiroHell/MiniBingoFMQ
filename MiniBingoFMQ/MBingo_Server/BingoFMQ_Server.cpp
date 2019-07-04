
#include "BingoFMQ_Server.h"

#include <iostream>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/optional/optional.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/core/ref.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

using ip::tcp;
using std::string;
using std::cin;
using std::cout;
using std::endl;


namespace FMQServer
{
	class CoordenatorManager : public boost::enable_shared_from_this<CoordenatorManager>//enable_shared_from_this is to keep our object alive for any operation that refers to it
	{
	private:
		tcp::socket sock_;
		std::string message = "Ola";
	public:

		//Properties
		const std::string& getMessage() const;
		void setMessage(std::string message);

	private:
		enum { max_length = 1024 };
		char data[max_length];

	public:
		typedef boost::shared_ptr<CoordenatorManager> pointer;
		CoordenatorManager(boost::asio::io_service& io_service) : sock_(io_service) {};

		// creating the pointer
		static pointer create(boost::asio::io_service& io_service)
		{
			return pointer(new CoordenatorManager(io_service));
		}

		//socket creation
		tcp::socket& socket()
		{
			return sock_;
		}

		void readMessage()
		{

			// starts the asynchronous receive operation of data from the socket.
			sock_.async_read_some(
				boost::asio::buffer(data, max_length),
				boost::bind(&CoordenatorManager::handle_read,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}

		void writeMessage()
		{
			//This function synchronously sends the buffer's data.
			//The function blocks until data is successfully sent, or an error occurs.
			sock_.async_write_some(
				boost::asio::buffer(message, max_length),
				boost::bind(&CoordenatorManager::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}


		void start()
		{
			readMessage();
			writeMessage();
		}

		void handle_read(const boost::system::error_code& err, size_t bytes_transferred)
		{
			if (!err) {
				cout << data << endl;
			}
			else {
				std::cerr << "error: " << err.message() << std::endl;
				sock_.close();
			}
		}

		void handle_write(const boost::system::error_code& err, size_t bytes_transferred)
		{
			if (!err) {
				cout << "Server sent message!" << endl;
			}
			else {
				std::cerr << "error: " << err.message() << endl;
				sock_.close();
			}
		}
	};

	const std::string& CoordenatorManager::getMessage() const
	{
		return message;
	}

	void CoordenatorManager::setMessage(std::string message)
	{
		this->message = message;
	}

	class Server
	{
	private:
		//This object is used to accepts client connections
		ip::tcp::acceptor acceptor_;
		void start_accept()
		{
			// socket
			CoordenatorManager::pointer connection =
				CoordenatorManager::create(acceptor_.get_io_service());

			// asynchronous accept operation and wait for a new connection.
			acceptor_.async_accept(connection->socket(),
				boost::bind(&Server::handle_accept, this, connection,
					boost::asio::placeholders::error));
		}
	public:
		//constructor for accepting connection from client
		Server(boost::asio::io_service& io_service) : acceptor_(io_service,
			tcp::endpoint(tcp::v4(), 1234))
		{
			start_accept();
		}
		void handle_accept(CoordenatorManager::pointer connection,
			const boost::system::error_code& err)
		{
			if (!err) {
				string i;
				cout << "Enter text\n\n\n";
				std::getline(cin, i);
				connection->setMessage(i);
				connection->start();
			}
			start_accept();
		}
	};

	void PrintMessage()
	{
		cout << "IM INSIDE SERVER";
	}

	auto ExecuteServer() -> void
	{
		try
		{
			boost::asio::io_service io_service;
			Server server(io_service);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << endl;
		}
	}
}

int main()
{
	FMQServer::ExecuteServer();
}
