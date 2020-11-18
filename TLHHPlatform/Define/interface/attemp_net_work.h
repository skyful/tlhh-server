#pragma once
class attemp_net_work
{
public:
	virtual bool	net_work_connected(const std::string& unique_id)=0;
	virtual bool	net_work_dis_connected(const std::string& unique_id)=0;
	virtual bool	net_work_read(const std::string& unique_id,const tlhh::tcp::command& command,const char* buffer,const size_t& size)=0;
};

typedef std::shared_ptr<attemp_net_work>		net_work_sptr_t;