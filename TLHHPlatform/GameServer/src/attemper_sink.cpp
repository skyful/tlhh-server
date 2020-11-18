#include "../include/attemper_sink.h"
#include "../include/define.h"
#include "../../Define/console.h"

#include <iostream>
attemper_sink::attemper_sink()
{

	work_service_ = std::make_shared<boost::asio::io_service>();
	work_work_ = std::make_unique<boost::asio::io_service::work>(*work_service_);
	work_thread_ = std::make_unique<std::thread>([this] {boost::system::error_code ec; work_service_->run(ec); });
	
	work_engine_ = std::make_unique<server_work_engine>(work_service_,SERVER_PORT);
	db_engine_ = std::make_unique<attemper_db>();
}


attemper_sink::~attemper_sink()
{
	work_work_.reset();
	work_thread_->join();
}

bool attemper_sink::net_work_read(const std::string& unique_id, const tlhh::tcp::command& command, const char* buffer, const size_t& size)
{ 
	CONSOLE_LOG(SERVER_LOG_NAME,tlhh::log_level::info, "recv unique_id : " << unique_id << " main: " << command.main_id << " sub: " << command.sub_id << " data:" << buffer);
	
	//work_engine_->send_data(unique_id, command.main_id, command.sub_id, buffer, size);
	return true;
}
bool attemper_sink::net_work_connected(const std::string& unique_id)
{
	return true;
}
bool attemper_sink::net_work_dis_connected(const std::string& unique_id)
{
	return true;
}
void attemper_sink::start()
{
	work_engine_->set_net_work_sink(shared_from_this());
	work_engine_->start_service();
	
	db_engine_->start();

	db_engine_->test_db();
	db_engine_->test_produce();
}