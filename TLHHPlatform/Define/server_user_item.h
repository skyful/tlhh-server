#pragma once

namespace tlhh {
	namespace tcp {
		class server_user_item
		{
		public:
			server_user_item() :
				is_robot_(false),
				client_unique_id_("")
			{

			}

		public:
			void reset()
			{
				is_robot_ = false;
				client_unique_id_.clear();
			}
			bool is_activity()
			{
				return !client_unique_id_.empty();
			}
			bool is_robot() { return is_robot_; }
			void set_client_unique_id(const std::string& unique_id) { client_unique_id_ = unique_id; }

		private:
			bool									is_robot_;
			std::string								client_unique_id_;

		};
	}
	
}
