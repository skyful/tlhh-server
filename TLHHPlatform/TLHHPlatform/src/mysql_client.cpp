#include <iostream>
#include <assert.h>
#include "../include/mysql_reset.h"
#include "../include/mysql_client.h"

#ifdef __WINDOWS
#include <WinSock.h>
#endif

mysql_client::mysql_client() :
	port_(3306),
	addr_("127.0.0.1"),
	user_("root"),
	password_(""),
	connected_(false),
	succeed_(false),
	database_(""),
	auto_reconnect_(false),
	mysql_(std::make_unique<MYSQL>())
{

}

mysql_client::~mysql_client()
{
	close();
}
bool mysql_client::connect(const int& port,
	const std::string& addr,
	const std::string& user,
	const std::string& pwd,
	const std::string& database)
{
	if (connected_)
		return true;
	connected_ = true;

	port_ = port;
	addr_ = addr;
	user_ = user;
	password_ = pwd;
	database_ = database;

	mysql_init(mysql_.get());

	unsigned int timeout = 10;	//超时时间10秒
	mysql_options(mysql_.get(), MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&timeout);//设置超时选项

	mysql_options(mysql_.get(), MYSQL_OPT_RECONNECT, &auto_reconnect_);
	std::cout << "reconnect enable:" << auto_reconnect_ << std::endl;

	succeed_ = connect_();
	
	return succeed_;
	
	
}
bool mysql_client::connect_()
{
	try {

		if(nullptr == mysql_real_connect(mysql_.get(), addr_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), port_, NULL, 0))
		{
			mysql_close(mysql_.get());
			if (mysql_errno(mysql_.get()))
			{
				std::cout << "connect db " << database_ << " error " << mysql_errno(mysql_.get()) << mysql_error(mysql_.get()) << std::endl;
			}
			return false;
		}
		else
		{
			if (!mysql_set_character_set(mysql_.get(), "GBK"))
			{
				printf("New client character set: %s\n",
					mysql_character_set_name(mysql_.get()));
			}
			return true;
		}
			

	}
	catch (...)
	{
		assert(false);
	}

	return false;
	
}
void mysql_client::check_reconnect()
{
	if (!auto_reconnect_)
	{
		int flag = mysql_errno(mysql_.get());
		if (flag == CR_SERVER_GONE_ERROR || flag == CR_SERVER_LOST)
		{
			mysql_close(mysql_.get());

			succeed_ = connect_();

			std::cout << "reconnect:" << succeed_ << std::endl;
		}
	}
	
}
sql_reset_sptr_t	mysql_client::executeQuery(const std::string& sql)
{
	if (!connected_)
		return nullptr;
	try
	{
		if (mysql_query(mysql_.get(), sql.c_str())) {
			check_reconnect();
			return nullptr;
		}
		MYSQL_RES* res = mysql_store_result(mysql_.get());
		if (!res)
		{
			std::cout << "Query failed:" << mysql_error(mysql_.get()) << std::endl;
			return nullptr;
		}
		std::shared_ptr<MYSQL_RES> reset(res, [](MYSQL_RES* res) {mysql_free_result(res); });
		std::shared_ptr<sql_reset> return_set = std::make_shared<mysql_reset>(reset, mysql_num_rows(res));
		return return_set;
		
	}
	catch (...)
	{
		return nullptr;
	}
	return nullptr;

	
}
/*
	执行sql语句
	执行 非select语句
	返回：>=0 受影响的行数  <0 查询失败
*/
long mysql_client::excuteModify(const std::string& sql)
{
	if (!connected_)
		return -1;
	try
	{
		
		if (mysql_query(mysql_.get(), sql.c_str())) {
			check_reconnect();
			return -1;
		}
		return mysql_affected_rows(mysql_.get());

	}
	catch (...)
	{
		return -1;
	}
	return -1;
}
void mysql_client::close()
{
	if (succeed_)
	{
		mysql_close(mysql_.get());
	}
}