#include "../include/attemper_sink.h"
#include "../include/define.h"
#include "../../Define/console.h"


attemper_sink::attemper_sink():
	work_engine_(std::make_unique<client_work_engine>(SERVER_ADDR,SERVER_PORT))
{
	CONSOLE_SET_PATH("E:/temp");
	CONSOLE_SAVE_LOG(tlhh::log_level::debug, tlhh::log_level::debug);
}


attemper_sink::~attemper_sink()
{
	work_engine_->dis_connect();
}
bool attemper_sink::net_work_connected()
{
	
	return true;
}
bool attemper_sink::net_work_dis_connected()
{
	return true;
}
bool attemper_sink::net_work_read( const tlhh::tcp::command& command, const char* buffer, const size_t& size)
{
	CONSOLE_LOG(CLIENT_LOG_NAME, tlhh::log_level::info, "recv main:" << command.main_id << " sub:" << command.sub_id << " data:" << buffer);
	return true;
}

void attemper_sink::connect()
{
	work_engine_->set_net_work_sink(shared_from_this());
	work_engine_->connect();
}
void attemper_sink::dis_connect()
{
	work_engine_->dis_connect();
}
void attemper_sink::send_data(const unsigned short& main_id, const unsigned short& sub_id, const char* data, const size_t& size)
{
	work_engine_->send_data(main_id, sub_id, data, size);
}
