#include  "../include/attemper_sink.h"
#include <iostream>
#include <array>
#include <random>
#include <memory>
#include <thread>
bool stop = false;

void test()
{
	std::shared_ptr<attemper_sink> attemper = std::make_shared<attemper_sink>();
	attemper->connect();
	std::default_random_engine e;
	while (!stop)
	{
		attemper->send_data(1, 1, "test data", 10);
		std::this_thread::sleep_for(std::chrono::milliseconds(100+ e() % 100));
		int rand = e() % 10;
		if (rand == 1)
		{
			attemper->dis_connect();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000+e() % 1000));
			attemper->connect();
		}
	}
}
int main()
{
	
	
	typedef  std::shared_ptr<std::thread>	thread_sptr_t;
	std::array<thread_sptr_t, 1000>	thread_group;
	int count = 1;
	for (int i = 0; i < count; i++)
	{
		thread_group.at(i) = std::make_shared<std::thread>(test);
	}
	std::cin.get();
	stop = true;
	for (int i = 0; i < count; i++)
	{
		if (thread_group.at(i))
		{
			thread_group.at(i)->join();
		}
		
	}
	return 0;
}