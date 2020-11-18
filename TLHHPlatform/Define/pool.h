#pragma once

#include <mutex>
#include <set>
#include <functional>
#include <algorithm>
namespace tlhh {

	template <class T>

	class pool final 
	{
	public:
		pool(const std::function< T()>& construct, const unsigned long long& next_size = 32):new_(construct), next_size_(next_size)
		{
			std::lock_guard<std::mutex> lock(mutext_);

			for (auto i = 0; i < next_size; ++i)
			{
				free_list_.emplace(new_());
			}
		}
		~pool()
		{
			std::lock_guard<std::mutex> lock(mutext_);

			free_list_.clear();

			busy_list_.clear();
		}

	public:
		void get(const std::function< void(const T&) >& op)
		{
			const T& t = get();
			if (t != nullptr) {
				op(t);
			}
			
		}
		T get()
		{

			std::lock_guard<std::mutex> lock(mutext_);

			if (free_list_.empty())
			{
				for (auto i = 0; i < next_size_; ++i)
				{
					free_list_.emplace(new_());
				}
			}
			if (!free_list_.empty())
			{
				auto p = busy_list_.emplace(*std::move(free_list_.begin()));
				free_list_.erase(free_list_.begin());
				return *p.first;
			}
			return nullptr;
		}
		void free(const T& t, const std::function< void(const T& )>& op = [](const T&) {})
		{
			std::lock_guard<std::mutex> lock(mutext_);
			auto it = busy_list_.find(t);
			if (it == busy_list_.end())
				return;
			free_list_.emplace(t);
			busy_list_.erase(it);
			op(t);

		}

		void each(const std::function<void(const T& )>&op = [](const T& ) {})
		{
			std::lock_guard<std::mutex> lock(mutext_);

			std::for_each(busy_list_.begin(), busy_list_.end(), [&op](const T& t) {
				op(t);
			});
		}
	private:
		unsigned long long		next_size_;

		std::mutex				mutext_;
		
		std::set<T>				busy_list_;
		
		std::set<T>				free_list_;

		std::function<T()>		new_;
	};
}