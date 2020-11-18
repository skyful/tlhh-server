#pragma once
#include <iostream>
#include <mutex>
#include <fstream>
#include <memory>

#include "const_define.h"
#include "utils.h"
#include "tlhh_define.h"

#define			MAX_LOG_FILE    10
#define			MAX_FILE_SIZE	5242880		//文件最大存储量 单位字节

namespace tlhh
{
	typedef					std::shared_ptr<std::ofstream>		fstream_sptr_t;
	enum log_level
	{
		debug,
		info,
		warn,
		error,
	};

	class console
	{
	public:
		~console()
		{
			close();
		}
		
	
	private:
		
		std::map<std::string, fstream_sptr_t>		file_set_;
		std::string									log_path_;
		io_service_uptr_t							file_io_service_;
		work_uptr_t									file_io_work_;
		thread_uptr_t								file_io_tread_;
		log_level									console_level_;
		log_level									file_level_;
	private:
		console():
			console_level_(debug),
			file_level_(warn)
		{
			file_io_service_ = std::make_unique<boost::asio::io_service>();
			file_io_work_ = std::make_unique<boost::asio::io_service::work>(*file_io_service_);
			file_io_tread_ = std::make_unique<std::thread>([this] {file_io_service_->run(); });
		}
		void debug_log(const log_level& level, const std::string& file_name, const std::string& s)
		{
			std::string str = tlhh::utis::get_date_time_externded_string() + " [debug] " + s + "\r\n";
			
			if(level>=file_level_)
				write_log(file_name, str);

			if(level>=console_level_)
				std::cout << str;

		}
		void info_log(const log_level& level, const std::string& file_name,  const std::string& s)
		{
			std::string str = tlhh::utis::get_date_time_externded_string() + " [info] " + s + "\r\n";
			
			if (level >= file_level_)
				write_log(file_name, str);

			if (level >= console_level_)
				std::cout << str;
		}
		void warn_log(const log_level& level, const std::string& file_name, const std::string& s)
		{
			std::string str = tlhh::utis::get_date_time_externded_string() + " [warn] " + s + "\r\n";
			
			if (level >= file_level_)
				write_log(file_name, str);

			if (level >= console_level_)
				std::cout << str;
		}
		void error_log(const log_level& level, const std::string& file_name,  const std::string& s)
		{
			std::string str = tlhh::utis::get_date_time_externded_string() + " [error] " + s + "\r\n";
			
			if (level >= file_level_)
				write_log(file_name, str);

			if (level >= console_level_)
				std::cout << str;
		}
		
		void write_log(const std::string& file_name, const std::string& str)
		{
			file_io_service_->post([this, file_name,str] {
				
				fstream_sptr_t file_handler = get_file_handler(file_name);

				if (file_handler == nullptr) return;

				file_handler->seekp(0, file_handler->end);
				long long file_size = file_handler->tellp();
				
				if (file_size >= MAX_FILE_SIZE)
				{//重新创建文件
					fstream_sptr_t new_file_handler = create_file(file_name);
					if (new_file_handler == nullptr)
						return;

					file_handler->flush();
					file_handler->close();

					file_set_[file_name] = nullptr;
					file_set_[file_name] = new_file_handler;
					file_handler = new_file_handler;
				}
				file_handler->write(str.c_str(),str.size());
				file_handler->flush();
			});
			
		}
		fstream_sptr_t create_file(const std::string& file_name)
		{
			std::string file_path = log_path_
				+ "/"
				+ file_name
				+ "_"
				+ tlhh::utis::get_date_time_string()
				+ ".log";
			std::cout << "file_path:" << file_path << std::endl;
			fstream_sptr_t	f_hanlder = std::make_shared<std::ofstream>(file_path,
				std::ios::out | std::ios::app);

			if (f_hanlder->fail())
			{//文件打开失败
				return nullptr;
			}
			return f_hanlder;
		}
		fstream_sptr_t get_file_handler(const std::string& file_name)
		{
			if (file_set_.find(file_name) != file_set_.end())
			{
				return file_set_[file_name];
			}
			if (file_set_.size() >= MAX_LOG_FILE)
				return nullptr;

			fstream_sptr_t file = create_file(file_name);
			if (file == nullptr)
				return nullptr;
			file_set_[file_name] = file;
			return file;
		}

		void close()
		{
			file_io_service_->post([this] {
				for (std::pair<std::string, fstream_sptr_t> pair : file_set_)
				{
					if (pair.second && pair.second->is_open())
					{ 

						pair.second->flush();
						pair.second->close();
					}
						
				}
			});
			file_io_work_.reset();
			file_io_tread_->join();
			file_io_tread_.reset();
			file_set_.clear();

		}
	public:
		
		static console* get_instance()
		{
			static	console*	console_ = nullptr;
			static std::mutex	instance_mutex_;

			if (console_ != nullptr)
			{
				return console_;
			}
			std::lock_guard<std::mutex> lock(instance_mutex_);
			if (console_ == nullptr)
			{
				console_ = new console();
			}

			return console_;
		}
		void set_log_path(const std::string& path)
		{
			log_path_ = path;
		}
		/**
		*	设置log存储等级	
			console_level	控制台显示最低等级
			file_level		文件存储最低等级
		**/
		void set_save_level(const log_level& console_level,const log_level& file_level)
		{
			console_level_ = console_level;
			file_level_ = file_level;
		}
		void log(const std::string& file_name,const log_level& level,const std::stringstream& s)
		{	
			switch (level)
			{
			case debug:
				debug_log(level,file_name,s.str());
				return;
			case info:
				info_log(level, file_name,s.str());
				return;
			case warn:
				warn_log(level, file_name,s.str());
				return;
			case error:
				error_log(level, file_name,s.str());
				return;
			}
		}
		
	};
	
}
#define CONSOLE_SET_PATH(path) \
	tlhh::console::get_instance()->set_log_path(path)

#define CONSOLE_SAVE_LOG(console_level,file_level) \
	tlhh::console::get_instance()->set_save_level(console_level,file_level)

#define CONSOLE_LOG(file_name,level,content) \
	tlhh::console::get_instance()->log(file_name,level,static_cast<std::stringstream&>(std::stringstream()<<std::right<<content))