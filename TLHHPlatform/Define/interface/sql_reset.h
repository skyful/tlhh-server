#pragma once
#include <string>
#include <memory>
#include <boost/any.hpp>

class sql_reset
{
public:
	virtual bool			is_eof() = 0;
	virtual	std::string		get_string(std::string name) = 0;
	virtual	std::string		get_string(int index) = 0;

	virtual int				get_int(std::string name) = 0;
	virtual int				get_int(int index) = 0;

	virtual unsigned int	get_uint(std::string name) = 0;
	virtual unsigned int	get_uint(int index) = 0;

	virtual long long		get_int64(std::string name) = 0;
	virtual long long		get_int64(int index) = 0;

	virtual unsigned long long get_uint64(std::string name) = 0;
	virtual unsigned long long get_uint64(int index) = 0;

	virtual bool			get_bool(std::string name) = 0;
	virtual bool			get_bool(int index) = 0;

	virtual double			get_double(std::string name) = 0;
	virtual double			get_double(int index) = 0;

	virtual size_t			get_row() = 0;
	virtual bool			next() = 0;
	virtual void			set_reset(std::shared_ptr<boost::any>)=0;

};

typedef std::shared_ptr<sql_reset>			sql_reset_sptr_t;