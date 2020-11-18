#pragma once
#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/asio/thread_pool.hpp>
#include <map>

#include "../../Define/platform.h"
#include "tcp_socket.h"
namespace tlhh {

	namespace tcp {

		
		
		typedef std::unique_ptr<tlhh::pool<tcp_client_sptr_t> >			pool_uptr_t;
		
		typedef std::unique_ptr<boost::asio::thread_pool>				thread_pool_uptr_t;

		class server : public boost::noncopyable {

		public:
			server();
			virtual ~server();
			
		public:
			void start(const unsigned int& port,
				const complete_handler_t& accept_handler = nullptr,
				const complete_handler_t& recv_handler = nullptr,
				const complete_handler_t& send_handler = nullptr,
				const complete_handler_t& close_handler = nullptr);

			void stop();
		public:
			void async_read(const tcp_client_sptr_t& client,
				buffer_sptr_t buffer,
				const size_t& expect_size,
				const unsigned long long&	millseconds,
				const pred_t& pred = [](const buffer_sptr_t&) { return true; });

			void async_write(const tcp_client_sptr_t& client, const std::string& data);
			
			void async_write(const tcp_client_sptr_t& client, buffer_sptr_t buffer);

			void broadcast(const std::string& data);

			void free_client(const tcp_client_sptr_t& client);
		private:
			void start_();
			void stop_();
			void accept_client();

			boost::asio::io_service& get_io_service();
			
			void handler_accept(const tcp_client_sptr_t& client, const boost::system::error_code& ec);
			
			void handler_read(const boost::any& client,
				const boost::system::error_code& ec,
				const size_t& bytes_transferred,
				const buffer_sptr_t& buffer);
			
			void handler_write(const boost::any& client,
				const boost::system::error_code& ec,
				const size_t& bytes_transferred,
				const buffer_sptr_t& buffer);

			
		private:
			unsigned int						port_;

			complete_handler_t					accept_handler_;
			complete_handler_t					recv_handler_;
			complete_handler_t					send_handler_;
			complete_handler_t					close_handler_;

			pool_uptr_t							client_pool_;
			thread_pool_uptr_t					thread_pool_;

			std::vector<io_service_uptr_t>		io_services_;
			std::atomic_size_t					io_service_pos_;
			std::vector<work_uptr_t>			io_service_works_;
			std::vector<thread_uptr_t>			io_service_threads_;
			std::map<std::string, tcp_client_sptr_t >	client_maps_;

			io_service_uptr_t					acceptor_io_service_;
			tcp_acceptor_uptr_t					acceptor_;
			work_uptr_t							acceptor_work_;
			thread_uptr_t						acceptor_thread_;

			
			bool								stoped_;
			bool								started_;

			std::mutex							stop_mutex_;
			std::mutex							accept_clients_mutex_;
			std::mutex							map_mutex_;
		};
	}
}
