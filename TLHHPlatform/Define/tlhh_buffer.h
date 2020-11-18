#pragma once
#include <vector>
#include <memory>
#include <atomic>
#include <functional>
#include <iterator>
#include <cstring>
namespace tlhh {
	class buffer final
	{
	public:
		buffer(const size_t& size = 0)
		{
			clear();
			alloc(size);
		}
		buffer(const std::string& data)
		{
			clear();
			*this = data;
			commit(data.length());
		}
		buffer(const char* dat,size_t size)
		{
			alloc(size);
			clear();	
			memcpy((void*)data(), dat, size);
			commit(size);
	
		}
		~buffer()
		{

		}
	public:
		bool alloc(const size_t& length)
		{
			if (length > max_size()) 
			{
				return false;
			}
			if (length < size())
			{
				return true;
			}
			buffer_.resize(length);
			return (size()>length) ? true : false;
		}
		void clear()
		{
			pos_ = 0;
			std::fill(buffer_.begin(), buffer_.end(), '\0');
		}
		char* memory()
		{
			return (size() == transferred()) ? nullptr : &buffer_[transferred()];
		}
		void commit(const size_t length)
		{
			pos_ = (length > (size() - transferred())) ? size() : (length + transferred());
		}
		const char* data() const
		{	
			return buffer_.data();
		}
		size_t max_size() const
		{
			return buffer_.max_size();
		}
		size_t size() const
		{
			return buffer_.size();
		}
		size_t transferred() const
		{
			return pos_;
		}
		size_t available() const
		{
			return size() - transferred();
		}
		std::vector<char>& raw()
		{
			return buffer_;
		}
		void get(const size_t& off, const size_t& count, const std::function< void(char*, const size_t&) >& op)
		{
			if (off > size())
			{
				return;
			}
			size_t min_count = size() - off;

			size_t real_count = (min_count < count) ? min_count : count;

			op(buffer_.data() + off, real_count);
		}
		void get(const size_t& count, const std::function<void(char*, const size_t&) >& op)
		{
			get(0, count, op);
		}
		void operator+(const std::string& str)
		{
			std::copy(str.cbegin(), str.cend(), std::back_inserter(buffer_));
		}

		void operator=(const std::string& str)
		{
			buffer_.assign(str.cbegin(), str.cend());
		}
	
	private:
		std::vector<char>				buffer_;
		std::atomic_size_t				pos_;
	};
}

typedef std::shared_ptr<tlhh::buffer>				buffer_sptr_t;
typedef std::function<bool(const buffer_sptr_t&)>	pred_t;