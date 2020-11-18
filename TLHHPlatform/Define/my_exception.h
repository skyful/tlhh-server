#pragma once
#include <exception>
struct my_exception :
	virtual	std::exception
{
public:
	my_exception(const unsigned short& ec, const std::string& msg):
		msg_(msg),
		ec_(ec)
	{
		

	}

	const std::string& get_msg()
	{
		return msg_;
	}
	const unsigned short& get_error()
	{
		return ec_;
	}
private:
	std::string msg_;
	unsigned short ec_;
};