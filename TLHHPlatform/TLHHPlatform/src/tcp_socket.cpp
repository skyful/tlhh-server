#include "../include/tcp_socket.h"
#include "../../Define/utils.h"
tlhh::tcp::socket::socket(boost::asio::io_service& io) :
socket_(std::make_unique<boost::asio::ip::tcp::socket>(io)),
param_(std::make_unique<tlhh::client_param>()),
strand_(std::make_unique<boost::asio::io_service::strand>(io)),
timer_(std::make_unique<boost::asio::steady_timer>(io))
{

}

tlhh::tcp::socket::~socket() 
{
	
}
void tlhh::tcp::socket::close_()
{
	if (!socket_->is_open())
	{
		return;
	}
	boost::system::error_code ec;
	socket_->shutdown(boost::asio::socket_base::shutdown_both, ec);
	socket_->close(ec);
}

void tlhh::tcp::socket::accept()
{
	//生成socket的唯一ID  ip:port

	std::lock_guard<std::mutex> lock(param_mutex_);

	boost::system::error_code ec;
	param_->reset();
	param_->unique_id = socket_->remote_endpoint(ec).address().to_string(ec) +
		":" +
		tlhh::utis::to_string<unsigned short>(socket_->remote_endpoint(ec).port()) +
		":" +
		tlhh::utis::to_string<std::time_t>(utis::get_time_stamp());
	

	

}
void tlhh::tcp::socket::free()
{
	std::lock_guard<std::mutex> lock(mutex_);

	cancel_timer();
	close_();
	param_->reset();
}
boost::asio::ip::tcp::socket&  tlhh::tcp::socket::raw_client()
{
	return *socket_;
}
void tlhh::tcp::socket::async_read(const client_handler_t& recv_handler,
	buffer_sptr_t						buffer,
	const size_t&						expect_size,
	const unsigned long long&				milliseconds_timeout,
	const pred_t&						pred)
{
	std::lock_guard<std::mutex> lock(mutex_);
	size_t expect_read = expect_size > buffer->size() ? buffer->size() : expect_size;

	if (!socket_->is_open())
	{
		return;
	}
	socket_->async_read_some(
		boost::asio::buffer(buffer->raw(), expect_read),
		strand_->wrap(boost::bind(&socket::handler_read,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			recv_handler,
			buffer,
			expect_read,
			pred
			)));
	if (milliseconds_timeout > 0)//超时实现
	{
		timer_->expires_from_now(std::chrono::milliseconds(milliseconds_timeout));
		timer_->async_wait(
			boost::bind(&socket::handler_timeout,
				shared_from_this(),
				boost::asio::placeholders::error,
				milliseconds_timeout));
	}
}
void tlhh::tcp::socket::handler_read(const boost::system::error_code& er,
	const size_t&									bytes_stransferred,
	const client_handler_t&							handler,
	const buffer_sptr_t&							buffer,
	const size_t&									expect_size,
	const pred_t&									pred
	)
{

	buffer->commit(bytes_stransferred);
	if (er || !bytes_stransferred || buffer->transferred() >= expect_size || pred(buffer))
	{
		cancel_timer();
		handler(shared_from_this(), er, bytes_stransferred ? buffer->transferred() : bytes_stransferred, buffer);
	}
	else
	{
		socket_->async_read_some(
			boost::asio::buffer(buffer->memory(), expect_size - buffer->transferred()),
			strand_->wrap(boost::bind(&socket::handler_read,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				handler,
				buffer,
				expect_size,
				pred)));
	}
}

void tlhh::tcp::socket::async_write(const client_handler_t& write_handler,
	const std::string&	str)
{
	std::lock_guard<std::mutex> lock(mutex_);

	buffer_sptr_t buffer = std::make_shared<tlhh::buffer>(str);

	if (!socket_->is_open())
	{
		return;
	}
	socket_->async_write_some(
		boost::asio::buffer(buffer->raw(), buffer->size()),
		strand_->wrap(boost::bind(&socket::handler_write,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			write_handler,
			buffer)));
}
void tlhh::tcp::socket::async_write(const client_handler_t& write_handler,
	const buffer_sptr_t&				buffer)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (!socket_->is_open())
	{
		return;
	}
	socket_->async_write_some(
		boost::asio::buffer(buffer->raw(), buffer->size()),
		strand_->wrap(boost::bind(&socket::handler_write,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			write_handler,
			buffer)));
}
void tlhh::tcp::socket::async_connect(const client_handler_t& connect_handler,
	const std::string&					server_addr,
	const unsigned short&				port)
{
	std::lock_guard<std::mutex> lock(mutex_);

	socket_->async_connect(
		boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(server_addr),port),
		strand_->wrap(boost::bind(&socket::handler_connect,
			shared_from_this(),
			boost::asio::placeholders::error,
			connect_handler)));
}

void tlhh::tcp::socket::handler_write(const boost::system::error_code& er,
	const size_t&							bytes_transferred,
	const client_handler_t&					handler,
	const buffer_sptr_t&					buffer)
{
	buffer->commit(bytes_transferred);
	if (er || !bytes_transferred || buffer->transferred() >= buffer->size())
	{
		handler(shared_from_this(), er, bytes_transferred ? buffer->transferred() : bytes_transferred, buffer);
	}
	else
	{
		socket_->async_write_some(
			boost::asio::buffer(buffer->memory(), buffer->available()),
			strand_->wrap(boost::bind(&socket::handler_write,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				handler,
				buffer))
			);
	}
}

void tlhh::tcp::socket::handler_connect(const boost::system::error_code& ec,
	const client_handler_t&							handler)
{
	handler(shared_from_this(),ec,0,nullptr);
}
void tlhh::tcp::socket::cancel_timer()
{
	boost::system::error_code ec;
	timer_->cancel(ec);
}
void tlhh::tcp::socket::handler_timeout(const boost::system::error_code& er, const unsigned long long& milliseconds)
{
	if (er)
		return;
	if (timer_->expires_at() < std::chrono::steady_clock::now())
	{
		cancel_timer();
		close_();
		return;
	}
	else if (milliseconds > 0)
	{

		timer_->async_wait(
			boost::bind(&socket::handler_timeout,
				shared_from_this(),
				boost::asio::placeholders::error,
				milliseconds));
	}
}