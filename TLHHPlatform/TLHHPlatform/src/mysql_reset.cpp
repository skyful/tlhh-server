#include "../include/mysql_reset.h"
#include <boost/lexical_cast.hpp>

mysql_reset::mysql_reset():
	rows_(0),
	sql_res_(nullptr)
{

}
mysql_reset::mysql_reset(const boost::any& reset,int rows):
	rows_(rows),
	sql_res_(nullptr)
{
	sql_res_ = boost::any_cast<std::shared_ptr<MYSQL_RES>>(reset);
}
mysql_reset::~mysql_reset()
{
	
}

void mysql_reset::set_reset(std::shared_ptr<boost::any> reset)
{
	
}
bool mysql_reset::is_eof()
{
	if (sql_res_ == nullptr)
		return false;
	sql_row_ = mysql_fetch_row(sql_res_.get());

	return !sql_row_;
}
std::string  mysql_reset::get_string(std::string name)
{
	return "";
}
std::string  mysql_reset::get_string(int index)
{
	if (!sql_row_)
		return "";
	return sql_row_[index];
}
int mysql_reset::get_int(std::string name)
{
	return 0;
}
int mysql_reset::get_int(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<int>(sql_row_[index]);
}
unsigned int mysql_reset::get_uint(std::string name)
{
	return 0;
}
unsigned int mysql_reset::get_uint(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<unsigned int>(sql_row_[index]);
}
long long mysql_reset::get_int64(std::string name)
{
	return 0;
}
long long mysql_reset::get_int64(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<long long>(sql_row_[index]);
}
unsigned long long mysql_reset::get_uint64(std::string name)
{
	return 0;
}
unsigned long long mysql_reset::get_uint64(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<unsigned long long>(sql_row_[index]);
}
bool mysql_reset::get_bool(std::string name)
{
	return false;
}
bool mysql_reset::get_bool(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<bool>(sql_row_[index]);
}
double mysql_reset::get_double(std::string name)
{
	return 0;
}
double mysql_reset::get_double(int index)
{
	if (!sql_row_)
		return 0;
	return  boost::lexical_cast<double>(sql_row_[index]);
}
size_t mysql_reset::get_row()
{
	return rows_;
}
bool mysql_reset::next()
{
	return false;
}