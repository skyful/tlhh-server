#pragma once
#include "../../Define/platform.h"
#include "../../Define/interface/attemp_net_work.h"
#include "../../TLHHPlatform/include/shared/server_work_engine.h"
#include "attemper_db.h"

class attemper_sink : public attemp_net_work,
	public std::enable_shared_from_this<attemper_sink>
{

	
public:
	attemper_sink();
	~attemper_sink();

public:
	void start();

public:
	virtual bool net_work_connected(const std::string& unique_id);
	virtual bool net_work_dis_connected(const std::string& unique_id);
	virtual bool net_work_read(const std::string& unique_id, const tlhh::tcp::command& command, const char* buffer, const size_t& size);

private:
	std::unique_ptr<server_work_engine>				work_engine_;
	std::unique_ptr<attemper_db>					db_engine_;
	io_service_sptr_t					work_service_;
	work_uptr_t							work_work_;
	thread_uptr_t						work_thread_;


};

