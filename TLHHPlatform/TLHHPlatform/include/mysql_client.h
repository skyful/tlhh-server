#pragma once
#include <string>
#include <functional>
#include <memory>
#ifdef __WINDOWS
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif
#include "../../Define/interface/sql_client.h"


class mysql_client :public sql_client
{
public:
	mysql_client();
	~mysql_client();

public:
	virtual bool connect(const int& port,
		const std::string& addr,
		const std::string& user,
		const std::string& pwd,
		const std::string& database);

	virtual void close();

	virtual sql_reset_sptr_t executeQuery(const std::string& sql);

	virtual long  excuteModify(const std::string& sql);
private:
	bool connect_();
	void check_reconnect();
private:
	int						port_;
	std::string				addr_;
	std::string				user_;
	std::string				password_;
	std::string				database_;
	bool					auto_reconnect_;

	bool					connected_;
	bool					succeed_;
	std::unique_ptr<MYSQL>	mysql_;
};
