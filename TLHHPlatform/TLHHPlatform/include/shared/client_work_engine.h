#pragma once
#include "../tcp_client.h"
#include "../../../Define/interface/attemp_client_work.h"
#include "../../../Define/buffer_parseh.h"
#include "../../../Define/app_api.h"

class  DLL_API client_work_engine
{
	typedef std::unique_ptr<tlhh::tcp::tcp_client>		tcp_client_uptr_t;
public:
	client_work_engine(const std::string server_addr,const unsigned int& port);
	~client_work_engine();

public:
	void connect();
	void dis_connect();
	void set_net_work_sink(const client_work_sptr_t& net_work);
	void send_data(
		const unsigned short& main_id,
		const unsigned short& sub_id,
		const char* data,
		size_t size);
private:
	void handler_connect(const tlhh::tcp::tcp_client_sptr_t&,
		const buffer_sptr_t&,
		const boost::system::error_code&,
		const size_t& size);
	void handler_recv(const tlhh::tcp::tcp_client_sptr_t&,
		const buffer_sptr_t&,
		const boost::system::error_code&,
		const size_t& size);
	void handler_send(const tlhh::tcp::tcp_client_sptr_t&,
		const buffer_sptr_t&,
		const boost::system::error_code&,
		const size_t& size);
	void handler_close(const tlhh::tcp::tcp_client_sptr_t&,
		const buffer_sptr_t&,
		const boost::system::error_code&,
		const size_t& size);

private:
	std::string						server_addr_;
	unsigned int					port_;

	tcp_client_uptr_t				client_;
	
	client_work_sptr_t					net_work_;
	buffer_parse_uptr_t				buffer_parse_;
	//客户端逻辑处理IO
	io_service_uptr_t				client_io_service_;
	work_uptr_t						client_work_;
	thread_uptr_t					client_thread_;
};