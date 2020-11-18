#pragma once
#include <mutex>
#include "../../Define/platform.h"
namespace tlhh 
{
	namespace tcp 
	{
	

		class socket final :public std::enable_shared_from_this<tlhh::tcp::socket>
		{
		public :
			socket(boost::asio::io_service& io);
			~socket();

		public:
			void accept();
			void free();
		private:
			void close_();

		public:
			void async_read(const client_handler_t& recv_handler,
				buffer_sptr_t						buffer,
				const size_t&						expect_size,
				const unsigned long long&				milliseconds_timeout,
				const pred_t&						pred);

			void async_write(const client_handler_t& write_handler,
				const std::string&					str);

			void async_write(const client_handler_t& write_handler,
				const buffer_sptr_t&				buffer);

			void async_connect(const client_handler_t& connect_handler,
				const std::string&					server_addr,
				const unsigned short&				port);

			boost::asio::ip::tcp::socket&  raw_client();
		private:
			void handler_read(const boost::system::error_code& er,
				const size_t&									bytes_stransferred,
				const client_handler_t&							handler,
				const buffer_sptr_t&							buffer,
				const size_t&									expect_size,
				const pred_t&									pred
				);

			void handler_write(const boost::system::error_code& er,
				const size_t&									byte_transferred,
				const client_handler_t&							handler,
				const buffer_sptr_t&							buffer
				);

			
			void handler_connect(const boost::system::error_code& ec,
				const client_handler_t&							handler);

			void handler_timeout(const boost::system::error_code& er,
				const unsigned long long&							milliseconds);


			void cancel_timer();

		public:
			param_ptr_t						param_;
		private:
			tcp_socket_uptr_t				socket_;
			std::mutex						mutex_;
			std::mutex						param_mutex_;
			strand_uptr_t					strand_;
			timer_uptr_t					timer_;
		};

		typedef std::shared_ptr<tlhh::tcp::socket>						tcp_client_sptr_t;

		typedef std::function<void(const tcp_client_sptr_t&,
			const buffer_sptr_t&,
			const boost::system::error_code&,
			const size_t& size)>										complete_handler_t;
	};
};