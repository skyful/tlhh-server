#include "../include/attemper_db.h"
#include <iostream>
attemper_db::attemper_db():
	test_manager_(std::make_unique<sql_manager>())
{

}
attemper_db::~attemper_db()
{

}
void attemper_db::start()
{
	db_io_service_ = std::make_unique<boost::asio::io_service>();
	db_work_ = std::make_unique<boost::asio::io_service::work>(*db_io_service_);
	db_thread_ = std::make_unique<std::thread>([this] {db_io_service_->run(); });

	test_manager_->connect(3306, "192.168.154.1", "root", "123456", "test");
}
void attemper_db::stop()
{
	test_manager_->close();
	
	db_work_.reset();
	db_thread_->join();
	db_io_service_.reset();
	db_thread_.reset();

}
void attemper_db::test_db()
{
	db_io_service_->post([this] {
		{
			sql_reset_sptr_t res_ptr = test_manager_->executeQuery("SELECT id,name FROM test.new_table");
			if (res_ptr == nullptr)
				return;
			std::cout << "db rows:" << res_ptr->get_row() << std::endl;
			while (!res_ptr->is_eof())
			{
				std::cout << "data:" << res_ptr->get_string(1) << std::endl;
			}
			std::cout << "test db end" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	});
}
void attemper_db::test_produce()
{
	db_io_service_->post([this] {
		sql_reset_sptr_t res_ptr = test_manager_->executeQuery("call test_insert('ÍõÎå')");
		if (res_ptr == nullptr)
			return;
		std::cout << "db rows:" << res_ptr->get_row() << std::endl;
		while (!res_ptr->is_eof())
		{
			std::cout << "data:" << res_ptr->get_string(0) << std::endl;
		}
		std::cout << "test db end" << std::endl;
	});
}