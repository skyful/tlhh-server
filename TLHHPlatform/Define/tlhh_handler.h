#pragma once

#include <boost/any.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>

#include "tlhh_buffer.h"

typedef std::function< void(const boost::any&	any,
	const boost::system::error_code&			ec,
	const size_t&								byte_transferred,
	const buffer_sptr_t&							buffer)>	client_handler_t;
