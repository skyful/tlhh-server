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
			unsigned char	data_kind;//��������
			unsigned char	data_check;//����У��
			size_t			data_size;//���峤��
		};
		struct command_head
		{
			struct command_info	info;
			struct command			command;
		};
		
#define  SIZE_COMMAND_HEAD			(sizeof(tlhh::tcp::command_head))
	}
}
