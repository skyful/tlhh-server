#include <iostream>
#include "../include/tcp_server.h"
tlhh::tcp::server::server() :
	port_(0),
	acceptor_io_service_(nullptr),
	acceptor_(nullptr),
	acceptor_work_(nullptr),
	acceptor_thread_(nullptr),
	accept_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {
		if (e)
		{
			return;
		}

		

	}),
	recv_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {

		if (e || !size)
			return;
		std::cout << " recv data:"<<buffer->data() << std::endl;


	}),
	send_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {

		if (e || !size)
			return;

	}),
	close_handler_([this](const tcp_client_sptr_t& client, const buffer_sptr_t& buffer, const boost::system::error_code& e, const size_t& size) {
		
	})

{
	stoped_ = true;
	started_ = false;

	for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
	{
		io_services_.emplace_back(std::make_unique<boost::asio::io_service>());
	}

	client_pool_ = std::make_unique<tlhh::pool<tcp_client_sptr_t> >([this] {

		return tcp_client_sptr_t(std::make_shared<tcp::socket>(get_io_service()));
	});

	thread_pool_ = std::make_unique<boost::asio::thread_pool>();
	
}
tlhh::tcp::server::~server()
{
	
}

void tlhh::tcp::server::start(const unsigned int& port,
	const complete_handler_t& accept_handler,
	const complete_handler_t& recv_handler,
	const complete_handler_t& send_handler,
	const complete_handler_t& close_handler)
{
	std::lock_guard<std::mutex> lock(stop_mutex_);

	if (started_)
		return;
	if (accept_handler)
		accept_handler_ = accept_handler;

	if (recv_handler)
		recv_handler_ = recv_handler;

	if (send_handler)
		send_handler_ = send_handler;

	if (close_handler)
		close_handler_ = close_handler;

	port_ = port;
	
	stoped_ = false;
	
	started_ = true;

	start_();
}
void tlhh::tcp::server::start_()
{
	acceptor_io_service_ = std::make_unique<boost::asio::io_service>();

	acceptor_work_ = std::make_unique<boost::asio::io_service::work>(*acceptor_io_service_);

	acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(*acceptor_io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_));

	acceptor_thread_ = std::make_unique<std::thread>([this] { boost::system::error_code ec; acceptor_io_service_->run(ec); });

	std::for_each(io_services_.begin(), io_services_.end(), [this](const io_service_uptr_t& io) 
	{
		io_service_works_.emplace_back(std::make_unique<boost::asio::io_service::work>(*io));
		io_service_threads_.emplace_back(std::make_unique<std::thread>([&io] {boost::system::error_code ec; io->run(ec); }));
	});

	accept_client();
}
void tlhh::tcp::server::stop()
{
	{
		std::lock_guard< std::mutex > lock_accept(accept_clients_mutex_);

		stoped_ = true;
	}

	std::lock_guard< std::mutex > lock_stop(stop_mutex_);

	if (!started_)
		return;

	stop_();

	started_ = false;
	
}

void tlhh::tcp::server::stop_()
{
	boost::system::error_code ec;

	acceptor_work_.reset();
	acceptor_->cancel(ec);
	acceptor_thread_->join();
	acceptor_io_service_->reset();
	acceptor_.reset();
	acceptor_work_.reset();
	//关闭所有客户端，并且取消timer
	client_pool_->each([this](const tcp_client_sptr_t& client) {
		client->free();
	});

	//等待所有IO完成
	io_service_works_.clear();
	std::for_each(io_service_threads_.begin(), io_service_threads_.end(), [](const thread_uptr_t& td) {td->join(); });
	std::for_each(io_services_.begin(), io_services_.end(), [](const io_service_uptr_t& io) {io->reset(); });
	io_service_threads_.clear();

	thread_pool_->join();
}
boost::asio::io_service& tlhh::tcp::server::get_io_service()
{
	boost::asio::io_service& io_service = *io_services_[io_service_pos_];

	++io_service_pos_;

	if (io_service_pos_ == io_services_.size())
	{
		io_service_pos_ = 0;
	}
	
	return io_service;
}
void tlhh::tcp::server::handler_accept(const tcp_client_sptr_t& client, const boost::system::error_code& ec)
{
	if (ec)
	{
		free_client(client);
	}
	else
		client->accept();
	std::cout << "accept:" << client->param_->unique_id << std::endl;;
	//继续监听
	accept_client();

	//执行逻辑业务
	accept_handler_(client, nullptr, ec, 1);

	if (!ec)
	{
		//开始recv
		buffer_sptr_t buff = std::make_shared<tlhh::buffer>(1024);
		buff->clear();
		async_read(client, buff, buff->size(), 0);
	}
	
}
void tlhh::tcp::server::handler_read(const boost::any& client,
	const boost::system::error_code& ec,
	const size_t& bytes_transferred,
	const buffer_sptr_t& buffer)
{
	const tcp_client_sptr_t& c = boost::any_cast<const tcp_client_sptr_t&>(client);
	
	if (ec || !bytes_transferred)
	{
		free_client(c);
	}
		

	/*boost::asio::post(*thread_pool_, [this, c, buffer, ec, bytes_transferred] {
		recv_handler_(c, buffer, ec, bytes_transferred);
	});*/
	recv_handler_(c, buffer, ec, bytes_transferred);

	if (!ec && bytes_transferred)
	{
		//继续接收
		buffer->clear();
		async_read(c, buffer, buffer->size(), 0);
	}
	

}

void tlhh::tcp::server::handler_write(const boost::any& client,
	const boost::system::error_code& ec,
	const size_t& bytes_transferred,
	const buffer_sptr_t& buffer)
{
	const tcp_client_sptr_t& c = boost::any_cast<const tcp_client_sptr_t&>(client);

	if (ec || !bytes_transferred)
	{
		free_client(c);
	}
		

	/*boost::asio::post(*thread_pool_, [this, c, buffer, ec, bytes_transferred] {
		send_handler_(c, buffer, ec, bytes_transferred);
	});*/

	send_handler_(c, buffer, ec, bytes_transferred);
}

void tlhh::tcp::server::free_client(const tcp_client_sptr_t& client)
{
	close_handler_(client, nullptr, boost::system::error_code(), 0);

	client_pool_->free(client, [this,client](const tcp_client_sptr_t& c) {
		c->free();
	});
	
}
void tlhh::tcp::server::accept_client()
{
	std::lock_guard<std::mutex> lock(accept_clients_mutex_);

	if (stoped_)
		return;

	client_pool_->get([this](const tcp_client_sptr_t& client) {

		acceptor_->async_accept(client->raw_client(),
			boost::bind(&server::handler_accept,
				this,
				client,
				boost::asio::placeholders::error));
	});
	
}

void tlhh::tcp::server::async_read(const tcp_client_sptr_t& client,
	buffer_sptr_t buffer,
	const size_t& expect_size,
	const unsigned long long&	millseconds,
	const pred_t& pred)
{
	client->async_read(std::bind(&server::handler_read,
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
void tlhh::tcp::server::async_write(const tcp_client_sptr_t& client, const std::string& data)
{
	client->async_write(std::bind(&server::handler_write,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4),
		data);
	

}
void tlhh::tcp::server::async_write(const tcp_client_sptr_t& client, buffer_sptr_t buffer)
{
	client->async_write(std::bind(&server::handler_write,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4),
		buffer);
}

void tlhh::tcp::server::broadcast(const std::string& data)
{
	
}

