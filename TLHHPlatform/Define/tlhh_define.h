#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include "tlhh_client_param.h"
#include "command.h"

typedef std::unique_ptr<boost::asio::io_service >				io_service_uptr_t;
typedef std::shared_ptr<boost::asio::io_service >				io_service_sptr_t;

typedef std::unique_ptr<boost::asio::io_service::work >			work_uptr_t;

typedef std::unique_ptr<std::thread >							thread_uptr_t;

typedef std::unique_ptr<boost::asio::ip::tcp::acceptor >		tcp_acceptor_uptr_t;

typedef std::unique_ptr<boost::asio::io_service::strand >		strand_uptr_t;

typedef std::unique_ptr<boost::asio::ip::tcp::socket >			tcp_socket_uptr_t;

typedef std::unique_ptr<std::string>							strng_uptr_t;

typedef std::unique_ptr<tlhh::client_param>						param_ptr_t;

typedef std::unique_ptr<boost::asio::steady_timer>				timer_uptr_t;




