#include "../include/shared/sql_manager.h"
#include "../include/mysql_client.h"
sql_manager::sql_manager():
	sql_client_(std::make_unique<mysql_client>())
{
}
sql_manager::~sql_manager()
{

}
bool sql_manager::connect(const int& port,
	const std::string& addr,
	const std::string& user,
	const std::string& pwd,
	const std::string& database)
{
	return sql_client_->connect(port,addr,user,pwd,database);
	
}
void sql_manager::close()
{
	sql_client_->close();
}
sql_reset_sptr_t  sql_manager::executeQuery(const std::string& sql)
{
	return sql_client_->executeQuery(sql);
}
long  sql_manager::excuteModify(const std::string& sql)
{
	return sql_client_->excuteModify(sql);
}