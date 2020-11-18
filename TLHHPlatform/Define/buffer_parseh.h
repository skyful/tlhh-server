#pragma once
#include "command.h"
#include "const_define.h"
#include "my_exception.h"
#include <functional>
#include <memory>
class buffer_parse
{
public:
	buffer_parse():
		recv_size(0)
	{

	}
	~buffer_parse()
	{

	}

private:
	char								recv_buffer_[SOCKET_TCP_BUFFER];
	size_t								recv_size;
public:
	void buffer_commit_unpack(const char* buffer, 
		const size_t& size,
		std::function<void(const tlhh::tcp::command& command,const char* data,const size_t& size)> cb)
	{
		char	recv_buffer[SOCKET_TCP_BUFFER];
		if (size > (SOCKET_TCP_BUFFER - recv_size)) throw my_exception(1, "数据包超长");

		memcpy(recv_buffer_ + recv_size, buffer, size);
		recv_size = recv_size+ size;

		while (recv_size >= sizeof(tlhh::tcp::command_head))
		{
			tlhh::tcp::command_head* head = (tlhh::tcp::command_head*)(recv_buffer_);
			size_t package_size = head->info.data_size;

			if (package_size < sizeof(tlhh::tcp::command_head)) throw my_exception(1, "数据包非法");
			if (package_size > SOCKET_TCP_BUFFER) throw my_exception(1, "数据包超长");
			if (recv_size < package_size)
				break;

			memcpy(recv_buffer, recv_buffer_, package_size);
			//如果需要解密，在此处将数据包解密
			size_t real_size = package_size;

			size_t data_size = real_size - sizeof(tlhh::tcp::command_head);
			char* data_buffer = recv_buffer + sizeof(tlhh::tcp::command_head);
			tlhh::tcp::command command = ((tlhh::tcp::command_head*)(recv_buffer))->command;
			
			if(cb)
				cb(command,(const char*) data_buffer, data_size);	
			
			recv_size -= package_size;
			if (recv_size > 0)
			{
				memcpy(recv_buffer_, recv_buffer_ + package_size, recv_size);
			}

		}
	}
	void buffer_pack(
		const unsigned short& main_id,
		const unsigned short& sub_id,
		const char* buffer,
		const size_t& size,
		std::function<void(const char* data, const size_t& size)> cb)
	{
		char	send_buffer[SOCKET_TCP_BUFFER];
		size_t	send_size=0;

		if (SIZE_COMMAND_HEAD + size > SOCKET_TCP_BUFFER)
		{//数据包太大
			throw my_exception(1, "发送数据太大");
		}
		//组装协议头
		tlhh::tcp::command_head command_head;
		command_head.info.data_check = 0x00;
		command_head.info.data_kind = 0x00;
		command_head.info.data_size = SIZE_COMMAND_HEAD + size;
		command_head.command.main_id = main_id;
		command_head.command.sub_id = sub_id;
		memcpy(send_buffer, &command_head, SIZE_COMMAND_HEAD);
		send_size += SIZE_COMMAND_HEAD;
		//组装包体
		memcpy(send_buffer + SIZE_COMMAND_HEAD, buffer, size);
		send_size += size;
		if(cb)
			cb(send_buffer, send_size);
	}
};

typedef std::unique_ptr<buffer_parse>					buffer_parse_uptr_t;