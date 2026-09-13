#pragma once

#include <sys/time.h>


class Clock
{
public:

	struct timeval tv_start;

	Clock()
	{
		gettimeofday(&tv_start, NULL);
	};

	uint64_t get_unix_epoch_ms()
	{
		uint64_t time_ms = 0;

		struct timeval tv;
		gettimeofday(&tv, NULL);

		time_ms = (uint64_t) tv.tv_sec * 1000;

		// time_ms += ((double)tv.tv_usec) / 1000000.0;
		time_ms +=  (uint64_t) (tv.tv_usec / 1000);

		return time_ms;
	}

	void print_tv_start()
	{
		printf("time s : %li \n", tv_start.tv_sec);
		printf("time us: %li \n", tv_start.tv_usec);
	}
};