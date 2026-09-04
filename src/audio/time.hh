#pragma once


#include <sys/time.h>



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


class SleepTimer
{
public:

	Clock clock;

	uint64_t start_time;
	uint64_t duration_ms;

	uint64_t end_time;


	SleepTimer() {}


	void sleep_ms(uint64_t ms)
	{
		usleep(ms * 1000);
	}


	void sleep(uint64_t _duration_ms)
	{
		start_time = clock.get_unix_epoch_ms();
		duration_ms = _duration_ms;
		end_time = start_time + duration_ms;

		uint64_t current_time_ms = clock.get_unix_epoch_ms();

		while(current_time_ms < end_time)
		{
			uint64_t delta_ms = end_time - current_time_ms;
			uint64_t sleep_time_ms = delta_ms * 0.5; // Do not sleep full duration to prevent 
			sleep_ms(sleep_time_ms);

			current_time_ms = clock.get_unix_epoch_ms();
		}
	}
};