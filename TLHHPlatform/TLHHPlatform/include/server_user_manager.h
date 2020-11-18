#pragma once
#include <map>
#include <atomic>
#include <memory>
#include "../../Define/pool.h"
#include "../../Define/server_user_item.h"
#include "../../Define/const_define.h"

typedef std::shared_ptr<tlhh::tcp::server_user_item>		user_item_sptr_t;
typedef std::unique_ptr<tlhh::pool<user_item_sptr_t> >		user_pool_uptr_t;
class server_user_manager
{
	
public:
	server_user_manager();
	~server_user_manager();

public:
	user_item_sptr_t activity_user(const std::string& unique_id);
	bool remove_user(const std::string& unique_id);
private:
	user_pool_uptr_t							user_pool_;
	std::map<std::string, user_item_sptr_t>		users_array_;
	std::atomic_size_t							user_count_;
};