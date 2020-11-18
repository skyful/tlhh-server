#pragma once
#include <map>
#include <mutex>
#include "../../../Define/app_api.h"
#include "../../../Define/interface/sql_client.h"
#include "../../../Define/interface/sql_reset.h"

typedef std::unique_ptr<sql_client>			sql_client_uptr_t;


class DLL_API sql_manager:public sql_client
{
public:
	sql_manager();
	~sql_manager();

public:
	virtual	bool connect(const int& port,
		const std::string& addr,
		const std::string& user,
		const std::string& pwd,
		const std::string& database);
	virtual void close();
	virtual sql_reset_sptr_t executeQuery(const std::string& sql);
	virtual long  excuteModify(const std::string& sql);
private:
	sql_client_uptr_t					sql_client_;
	sql_reset_sptr_t					sql_reset_;
};

typedef std::unique_ptr<sql_manager>		sql_manager_uptr_t;