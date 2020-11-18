#include "../include/tcp_client.h"

tlhh::tcp::tcp_client::tcp_client(const std::string& server_addr, const unsigned int& port,
	const complete_handler_t& connected_handler,
	const complete_handler_t& recv_handler,
	const complete_handler_t& send_handler,
	const complete_handler_t& close_handler):

	client_io_service_(nullptr),
	client_(nullptr),
	connected_(false),
	connected_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {
		if (e)
		{
			return;
		}

	}),
	recv_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {

		if (e || !size)
			return;
		


	}),
	send_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {

		if (e || !size)
			return;

	}),
	close_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {

	})

{
	if (connected_handler)
		connected_handler_ = connected_handler;
	if (recv_handler)
		recv_handler_ = recv_handler;
	if (send_handler)
		send_handler_ = send_handler;
	if (close_handler)
		close_handler_ = close_handler;

	server_addr_ = server_addr;
	port_ = port;
}
tlhh::tcp::tcp_client::~tcp_client()
{
	dis_connect();
}


void tlhh::tcp::tcp_client::handler_connected(const boost::any& client,
	const boost::system::error_code& ec,
	const size_t& bytes_transferred,
	const buffer_sptr_t& buffer)
{
	tcp_client_sptr_t c = boost::any_cast<tcp_client_sptr_t>(client);

	if (ec)
	{
		free_client(c);
	}

	connected_handler_(c, nullptr, ec, 1);

	if (!ec)
	{
		buffer_sptr_t buffer_tmp = std::make_shared<tlhh::buffer>(1024);
		buffer_tmp->clear();
		async_read(buffer_tmp, buffer_tmp->size(), 0);
	}
	
		
}

void tlhh::tcp::tcp_client::handler_read(const boost::any& client,
	const boost::system::error_code& ec,
	const size_t& bytes_transferred,
	const buffer_sptr_t& buffer)
{
	const tcp_client_sptr_t& c = boost::any_cast<tcp_client_sptr_t>(client);

	if (ec || !bytes_transferred)
	{
		free_client(c);
	}
	recv_handler_(c, buffer, ec, bytes_transferred);
	if (!ec && bytes_transferred)
	{
		//¼ÌÐø½ÓÊÜ
		buffer->clear();
		async_read(buffer, buffer->size(), 0);
	}
	
}

void tlhh::tcp::tcp_client::handler_write(const boost::any& client,
	const boost::system::error_code& ec,
	const size_t& bytes_transferred,
	const buffer_sptr_t& buffer)
{
	const tcp_client_sptr_t& c = boost::any_cast<tcp_client_sptr_t>(client);
	if (ec || !bytes_transferred)
	{
		free_client(c);
	}
	send_handler_(c, buffer, ec, bytes_transferred);
}

void tlhh::tcp::tcp_client::async_read(
	buffer_sptr_t buffer,
	const size_t& expect_size,
	const unsigned long long& millseconds,
	const pred_t& pred )
{
	client_->async_read(std::bind(&tcp_client::handler_read,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4),
		buffer,
		expect_size,
		millseconds,
		pred);
}

void tlhh::tcp::tcp_client::async_write(const std::string& data)
{
	client_->async_write(std::bind(&tcp_client::handler_write,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4),
		data);
}
void tlhh::tcp::tcp_client::async_write( const buffer_sptr_t& buffer)
{
	client_->async_write(std::bind(&tcp_client::handler_write,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4),
		buffer);
}

void tlhh::tcp::tcp_client::connect()
{
	std::lock_guard<std::mutex> lock(stop_mutex_);

	if (connected_)
		return;

	
	connect_();
	connected_ = true;
}

void tlhh::tcp::tcp_client::connect_()
{
	client_io_service_ = std::make_unique<boost::asio::io_service>();
	client_work_ = std::make_unique < boost::asio::io_service::work >(*client_io_service_);
	client_thread_ = std::make_unique<std::thread>([this] {boost::system::error_code ec; client_io_service_->run(ec); });

	client_ = std::make_shared<tlhh::tcp::socket>(*client_io_service_);

	client_->async_connect(std::bind(&tcp_client::handler_connected,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3, 
		std::placeholders::_4),
		server_addr_,
		port_);
}
void tlhh::tcp::tcp_client::dis_connect()
{
	std::lock_guard<std::mutex> lock(stop_mutex_);

	if (!connected_)
		return;
	
	dis_connect_();

	connected_ = false;
	
}
void tlhh::tcp::tcp_client::dis_connect_()
{
	free_client(client_);
	client_work_.reset();
	client_thread_->join();
	client_io_service_.reset();
	client_.reset();
}
void tlhh::tcp::tcp_client::free_client(const tcp_client_sptr_t& client)
{
	client->free();
}

bool tlhh::tcp::tcp_client::is_connect()
{
	return connected_;
}