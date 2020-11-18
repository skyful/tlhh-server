#pragma once

#include "../../Define/platform.h"
#include "../../Define/interface/attemp_client_work.h"
#include "../../TLHHPlatform/include/shared/client_work_engine.h"


class attemper_sink : public attemp_client_work,
	public std::enable_shared_from_this<attemper_sink>
{
public:
	attemper_sink();
	~attemper_sink();

public:
	virtual bool	net_work_connected();
	virtual bool	net_work_dis_connected();
	virtual bool	net_work_read(const tlhh::tcp::command& command, const char* buffer, const size_t& size);
public:
	void connect();
	void dis_connect();
	void send_data(const unsigned short& main_id,const unsigned short& sub_id,const char* data,const size_t& size);
private:
	std::unique_ptr<client_work_engine>				work_engine_;
};

