#pragma once
namespace tlhh {
	class client_param final
	{
	public:
		client_param()
		{
			reset(); 
		}
		~client_param(){}
	public:
		void reset()
		{
			unique_id.clear();
		}


	public:
		std::string unique_id;
	};
}