#include <assert.h>
#include "../include/server_user_manager.h"

server_user_manager::server_user_manager():
	user_count_(0)
{
	user_pool_ = std::make_unique<tlhh::pool<user_item_sptr_t> >([this] {
		return user_item_sptr_t(std::make_shared<tlhh::tcp::server_user_item>());
		},MAX_USER_COUNT);
}
server_user_manager::~server_user_manager()
{

}

user_item_sptr_t server_user_manager::activity_user(const std::string& unique_id)
{
	assert(!unique_id.empty());
	if (user_count_ >= MAX_USER_COUNT)
	{
		return nullptr;
	}
	if (users_array_.find(unique_id) != users_array_.end())
	{
		return users_array_[unique_id];
	}
	const user_item_sptr_t& user =  user_pool_->get();

	if (user == nullptr)
		return nullptr;

	users_array_[unique_id] = user;
	return user;
}

bool server_user_manager::remove_user(const std::string& unique_id)
{
	assert(!unique_id.empty());
	if (users_array_.find(unique_id) == users_array_.end())
	{
		return false;;
	}
	
	user_pool_->free(users_array_[unique_id]);
	users_array_.erase(unique_id);
	return true;
}