#include <iostream>
#include "../include/shared/server_work_engine.h"
#include "../../Define/my_exception.h"
#include "../../Define/command_define.h"

server_work_engine::server_work_engine(const io_service_sptr_t& io, const unsigned int& port) :
	server_(std::make_unique<tlhh::tcp::server>()),
	users_manager_(std::make_unique<server_user_manager>()),
	buffer_parse_(std::make_unique<buffer_parse>()),
	port_(port),
	work_io_(io)
{
	
}


server_work_engine::~server_work_engine()
{
}

void server_work_engine::start_service()
{
	server_->start(port_,
		[this] (const tlhh::tcp::tcp_client_sptr_t& client,
			const buffer_sptr_t& buffer,
			const boost::system::error_code& ec,
			const size_t&		size){
				accept_handler_(client, buffer, ec, size);
			},
		[this](const tlhh::tcp::tcp_client_sptr_t& client,
			const buffer_sptr_t& buffer,
			const boost::system::error_code& ec,
			const size_t&		size) {
				recv_handler_(client, buffer, ec, size);
			},
		[this](const tlhh::tcp::tcp_client_sptr_t& client,
			const buffer_sptr_t& buffer,
			const boost::system::error_code& ec,
			const size_t&		size) {
				send_handler_(client, buffer, ec, size);
			},
		[this](const tlhh::tcp::tcp_client_sptr_t& client,
			const buffer_sptr_t& buffer,
			const boost::system::error_code& ec,
			const size_t&		size) {
				close_handler_(client, buffer, ec, size);
			}
		);
}

void server_work_engine::set_net_work_sink(const net_work_sptr_t& net_work)
{
	net_work_ = net_work;
}
void server_work_engine::stop_service()
{
	server_->stop();
}
void server_work_engine::accept_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t&		size)
{
	work_io_->post([this, client,size,ec] {
		if (ec || !size)
			return;
		if (net_work_ != nullptr)
		{
			net_work_->net_work_connected(client->param_->unique_id);
		}

		if (users_map_.find(client->param_->unique_id) == users_map_.end())
		{
			users_map_[client->param_->unique_id] = client;
		}
	});
	
		
}
void server_work_engine::close_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t&		size)
{
	work_io_->post([this,client] {
		std::string unique_id = client->param_->unique_id;
		if (net_work_ != nullptr)
		{
			net_work_->net_work_dis_connected(unique_id);
		}

		if (users_map_.find(unique_id) != users_map_.end())
		{
			users_map_.erase(unique_id);
		}
	});
	
		
}
void server_work_engine::send_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t&		size)
{

}
void server_work_engine::recv_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
	const buffer_sptr_t& buffer,
	const boost::system::error_code& ec,
	const size_t&		size)
{
	if (ec || !size)
		return;

	
	buffer_sptr_t buffer_tmp = std::make_shared<tlhh::buffer>(buffer->data(), buffer->transferred());
	work_io_->post([this, buffer_tmp, ec, client] {
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
					if (command.sub_id == CMD_SUB_KERNEL_HEART)//ÐÄÌø°ü
					{

					}
				}
				else
				{
					if (net_work_ != nullptr)
					{
						bool success = net_work_->net_work_read(client->param_->unique_id, command, data, size);
						if (!success)
						{
							server_->free_client(client);
						}
					}
				}
			});

		}
		catch (my_exception e)
		{
			server_->free_client(client);
		}
	});

	
		
}


void server_work_engine::send_data(const std::string& unique_id,
	const unsigned short& main_id, 
	const unsigned short& sub_id,
	const char* data, 
	const size_t& size)
{
	try
	{
		buffer_parse_->buffer_pack(main_id, sub_id, data, size, [this, unique_id](const char* data, const size_t& size) {
			buffer_sptr_t buffer = std::make_shared<tlhh::buffer>(data, size);
			if (users_map_.find(unique_id) != users_map_.end())
			{
				server_->async_write(users_map_[unique_id], buffer);
			}
		});
	}
	catch (my_exception e)
	{

	}
	
	
	
}