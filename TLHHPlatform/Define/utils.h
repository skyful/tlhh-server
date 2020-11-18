#pragma once

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
namespace tlhh
{
	namespace utis
	{
		template<class T>
		inline std::string to_string(const T& v)
		{
			std::stringstream s;
			s << v;
			return s.str();
		}

		//ʱ��� ����
		inline std::time_t	get_time_stamp()
		{
			std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
			std::time_t time_stamp = tmp.count();
			return time_stamp;
		}
		//���� ��-��-�� ʱ:��:��:����
		inline std::string	get_date_time_externded_string()
		{

			std::time_t	tm_mill = get_time_stamp();
			std::time_t	tm = tm_mill / 1000;
			struct tm t_m;
#ifdef __WINDOWS
			localtime_s(&t_m, &tm);
			
#else
			localtime_r( &tm, &t_m);
#endif
			char data[25] = { 0 };

			snprintf(data, sizeof(data), "%d-%02d-%02d %02d:%02d:%02d:%03lld",
				t_m.tm_year + 1900,
				t_m.tm_mon + 1,
				t_m.tm_mday,
				t_m.tm_hour,
				t_m.tm_min,
				t_m.tm_sec,
				(long long int)tm_mill % 1000);

			return data;
		}
		//���� ������ʱ����
		inline std::string get_date_time_string()
		{
			std::time_t	tm = get_time_stamp()/1000;
			struct tm t_m;
#ifdef __WINDOWS
			localtime_s(&t_m, &tm);
#else
			localtime_r(&tm, &t_m);
			
#endif
			char data[15] = { 0 };

			snprintf(data, sizeof(data), "%d%02d%02d%02d%02d%02d",
				t_m.tm_year + 1900,
				t_m.tm_mon + 1,
				t_m.tm_mday,
				t_m.tm_hour,
				t_m.tm_min,
				t_m.tm_sec);

			return data;
		}
		 
	};
}