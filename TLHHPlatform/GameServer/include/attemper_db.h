#pragma once
#include "../../TLHHPlatform/include/shared/sql_manager.h"
#include "../../Define/tlhh_define.h"
class attemper_db
{
public:
	attemper_db();
	~attemper_db();

public:
	void start();
	void stop();
	void test_db();
	void test_produce();
private:
	sql_manager_uptr_t						test_manager_;

	io_service_uptr_t						db_io_service_;
	work_uptr_t								db_work_;
	thread_uptr_t							db_thread_; 

};