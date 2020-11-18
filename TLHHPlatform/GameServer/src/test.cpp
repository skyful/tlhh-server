#include <iostream>
#include "../include/attemper_sink.h"
#include "../../Define/console.h"
#include "../include/attemper_db.h"
int main()
{
	CONSOLE_SET_PATH("E:/temp");

	std::shared_ptr<attemper_sink>  sink_=std::make_shared<attemper_sink>();
	sink_->start();


	std::cin.get();
	return 0;
}