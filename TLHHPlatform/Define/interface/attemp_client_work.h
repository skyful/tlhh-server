#pragma once
class attemp_client_work
{
public:
	virtual bool	net_work_connected()=0;
	virtual bool	net_work_dis_connected()=0;
	virtual bool	net_work_read(const tlhh::tcp::command& command, const char* buffer, const size_t& size) = 0;
};

typedef std::shared_ptr<attemp_client_work>		client_work_sptr_t;