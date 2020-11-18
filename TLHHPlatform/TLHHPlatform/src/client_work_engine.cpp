#include "../include/shared/client_work_engine.h"
#include "../../Define/command_define.h"
#include "../../Define/my_exception.h"


client_work_engine::client_work_engine(const std::string server_addr, const unsigned int& port) :
	server_addr_(server_addr),
	port_(port),
	buffer_parse_(std::make_unique<buffer_parse>())
{
	client_ = std::make_unique < tlhh::tcp::tcp_client >(server_addr_, port_,
		std::bind(&client_work_engine::handler_connect, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4),
		std::bind(&client_work_engine::handler_recv, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4),
		std::bind(&client_work_engine::handler_send, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4),
		std::bind(&client_work_engine::handler_close, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4));

	
}
client_work_engine::~client_work_engine()
{

}

void client_work_engine::connect()
{
	client_io_service_ = std::make_unique<boost::asio::io_service>();
	client_work_ = std::make_unique<boost::asio::io_service::work>(*client_io_service_);
	client_thread_ = std::make_unique<std::thread>([this] {boost::system::error_code ec; client_io_service_->run(ec); });

	client_->connect();
}
void client_work_engine::dis_connect()
{
	if (!client_->is_connect())
	{
		return;
	}
	client_->dis_connect();

	client_work_.reset();
	client_thread_->join();
	client_io_service_.reset();
	client_thread_.reset();
}

void client_work_engine::handler_connect(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t& size)
{
	if (ec || !size)
	{//连接失败
		return;
	}
	if (net_work_ != nullptr)
	{
		net_work_->net_work_connected();
	}
}
void client_work_engine::handler_close(const tlhh::tcp::tcp_client_sptr_t&,
	const buffer_sptr_t&,
	const boost::system::error_code&,
	const size_t& size)
{
	if (net_work_ != nullptr)
	{
		net_work_->net_work_dis_connected();
	}
}
void client_work_engine::handler_send(const tlhh::tcp::tcp_client_sptr_t&,
	const buffer_sptr_t&,
	const boost::system::error_code&,
	const size_t& size)
{

}

void client_work_engine::handler_recv(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t& size)
{
	if (ec || !size)
		return;


	buffer_sptr_t buffer_tmp = std::make_shared<tlhh::buffer>(buffer->data(), buffer->transferred());

	client_io_service_->post([this, client, buffer_tmp, ec, size] {
		if (ec)
		{
			return;
		}
		try {
			buffer_parse_->buffer_commit_unpack(buffer_tmp->data(), buffer_tmp->transferred(),
				[this, client](const tlhh::tcp::command& command, const char* data, const size_t& size)
			{
				if (command.main_id == CMD_MAIN_KERNEL)
				{
					if (command.sub_id == CMD_SUB_KERNEL_HEART)//心跳包
					{

					}
				}
				else
				{
					if (net_work_ != nullptr)
					{
						bool success = net_work_->net_work_read(command, data, size);
						if (!success)
						{
							
						}
					}
				}
			});


		}
		catch (my_exception e)
		{
			
		}


	});
}

void client_work_engine::set_net_work_sink(const client_work_sptr_t& net_work)
{
	net_work_ = net_work;
}


void client_work_engine::send_data(
	const unsigned short& main_id,
	const unsigned short& sub_id,
	const char* data,
	size_t size)
{
	try {
		buffer_parse_->buffer_pack(main_id, sub_id, data, size, [this](const char* data, const size_t& size) {
			buffer_sptr_t buffer = std::make_shared<tlhh::buffer>(data, size);
			client_->async_write(buffer);
		});
	}
	catch (my_exception e)
	{
		
	}
	


}