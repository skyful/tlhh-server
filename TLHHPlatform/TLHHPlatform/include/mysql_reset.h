#pragma once

#include "../../Define/interface/sql_reset.h"
#include <memory>
#include <boost/any.hpp>
#ifdef __WINDOWS
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif
class mysql_reset:public sql_reset
{
public:
	mysql_reset();
	mysql_reset(const boost::any& reset,int rows);
	~mysql_reset();

public:
	virtual bool			is_eof();
	virtual	std::string		get_string(std::string name);
	virtual	std::string		get_string(int index);
	virtual int				get_int(std::string name);
	virtual int				get_int(int index);
	virtual unsigned int	get_uint(std::string name);
	virtual unsigned int	get_uint(int index);
	virtual long long		get_int64(std::string name);
	virtual long long		get_int64(int index);
	virtual unsigned long long get_uint64(std::string name);
	virtual unsigned long long get_uint64(int index);
	virtual bool			get_bool(std::string name);
	virtual bool			get_bool(int index);
	virtual double			get_double(std::string name);
	virtual double			get_double(int index);
	virtual size_t			get_row();
	virtual bool			next();
	virtual void			set_reset(std::shared_ptr<boost::any>);

private:
	int								rows_;
	std::shared_ptr<MYSQL_RES>		sql_res_;
	MYSQL_ROW						sql_row_;
};