#pragma once
#include "../tcp_server.h"
#include "../server_user_manager.h"
#include "../../../Define/buffer_parseh.h"
#include "../../../Define/app_api.h"
#include "../../../Define/interface/attemp_net_work.h"

class DLL_API server_work_engine
{
	typedef std::unique_ptr<tlhh::tcp::server>		server_uptr_t;
	typedef std::unique_ptr<server_user_manager>	user_mananger_uptr_t;

public:
	server_work_engine( const io_service_sptr_t& io ,const unsigned int& port);
	~server_work_engine();

public:
	void start_service();
	void stop_service();
public:
	void set_net_work_sink(const net_work_sptr_t& net_work);
	void send_data(const std::string& unique_id,
		const unsigned short& main_id,
		const unsigned short& sub_id,
		const char* data, 
		const size_t& size);

private:
	void accept_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
		const buffer_sptr_t& buffer,
		const boost::system::error_code& ec,
		const size_t&		size);

	void recv_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
		const buffer_sptr_t& buffer,
		const boost::system::error_code& ec,
		const size_t&		size);

	void send_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
		const buffer_sptr_t& buffer,
		const boost::system::error_code& ec,
		const size_t&		size);

	void close_handler_(const tlhh::tcp::tcp_client_sptr_t& client,
		const buffer_sptr_t& buffer,
		const boost::system::error_code& ec,
		const size_t&		size);
	
private:

	server_uptr_t						server_;

	io_service_sptr_t					work_io_;

	net_work_sptr_t						net_work_;
	buffer_parse_uptr_t					buffer_parse_;

	user_mananger_uptr_t				users_manager_;

	std::map<std::string, tlhh::tcp::tcp_client_sptr_t> users_map_;

	unsigned	int						port_;

};

