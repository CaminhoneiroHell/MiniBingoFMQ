#pragma once
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

namespace FMQServer
{
	void PrintMessage();

	void ExecuteServer();

}
