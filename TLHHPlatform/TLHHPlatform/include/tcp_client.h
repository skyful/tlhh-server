#pragma once
#include "../../Define/platform.h"
#include "tcp_socket.h"
namespace tlhh {
	namespace tcp {
		class tcp_client
		{
			
		public:
			tcp_client(const std::string& server_addr, const unsigned int& port,
				const complete_handler_t& connected_handler,
				const complete_handler_t& recv_handler,
				const complete_handler_t& send_handler,
				const complete_handler_t& close_handler);
			~tcp_client();

		public:
			void connect();

			void dis_connect();
			
			bool is_connect();
		public:
			void async_read(
				buffer_sptr_t buffer,
				const size_t& expect_size,
				const unsigned long long& millseconds,
				const pred_t& pred = [](const buffer_sptr_t&) { return true; });

			void async_write( const std::string& data);
			void async_write( const buffer_sptr_t& buffer);

		private:
			void connect_();
			void dis_connect_();

			void handler_connected(const boost::any& client,
				const boost::system::error_code& ec,
				const size_t& bytes_transferred,
				const buffer_sptr_t& buffer);

			void handler_read(const boost::any& client,
				const boost::system::error_code& ec,
				const size_t& bytes_transferred,
				const buffer_sptr_t& buffer);

			void handler_write(const boost::any& client,
				const boost::system::error_code& ec,
				const size_t& bytes_transferred,
				const buffer_sptr_t& buffer);

			void free_client(const tcp_client_sptr_t& client);
		private:
			bool								connected_;
			complete_handler_t					connected_handler_;
			complete_handler_t					recv_handler_;
			complete_handler_t					send_handler_;
			complete_handler_t					close_handler_;

			io_service_uptr_t					client_io_service_;
			work_uptr_t							client_work_;
			thread_uptr_t						client_thread_;

			tcp_client_sptr_t					client_;
			std::string							server_addr_;
			unsigned int						port_;

			std::mutex							stop_mutex_;
		};

	}
}