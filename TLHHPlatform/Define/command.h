#pragma once
namespace tlhh {
	namespace tcp {


		struct command
		{
			unsigned short	main_id;
			unsigned short	sub_id;
		};

		struct command_info 
		{
			unsigned char	data_kind;//数据类型
			unsigned char	data_check;//数据校验
			size_t			data_size;//包体长度
		};
		struct command_head
		{
			struct command_info	info;
			struct command			command;
		};
		
#define  SIZE_COMMAND_HEAD			(sizeof(tlhh::tcp::command_head))
	}
}
