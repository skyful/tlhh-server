#pragma once
#include <string>
#include <memory>
#include <boost/any.hpp>
#include "sql_reset.h"
class sql_client
{
public:
	virtual	bool connect(const int& port,
		const std::string& addr,
		const std::string& user,
		const std::string& pwd,
		const std::string& database)=0;
	
	virtual void close()=0;

	virtual sql_reset_sptr_t executeQuery(const std::string& sql)=0;

	virtual long  excuteModify(const std::string& sql)=0;


};
