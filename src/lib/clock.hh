#pragma once

#include <sys/time.h>
#include <stdio.h>
#include <cstdint>

#include "lib/defs.hh"
#include "lib/print.hh"

class Clock
{
public:

	struct timeval tv_start;
	struct timeval tv_last;

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

	i64 get_unix_epoch_us()
	{
		i64 time_us = 0;

		gettimeofday(&tv_last, NULL);

		time_us = (i64) tv_last.tv_sec * (i64)1000000;

		// time_ms += ((double)tv.tv_usec) / 1000000.0;
		time_us +=  (i64) tv_last.tv_usec;

		return time_us;
	}

	void print_tv_start()
	{
		printf("time s : %li \n", tv_start.tv_sec);
		printf("time us: %li \n", tv_start.tv_usec);
	}

	void print_current_m_sec()
	{
		
		printf("Microsecond: %li \n", get_unix_epoch_ms());
	}
};

struct Stopwatch
{
    struct timeval tv_start;
	struct timeval tv_stop;

    bool running = false;

    void start()
    {
        running = true;
        gettimeofday(&tv_start, NULL);    
    }

    void stop()
    {
        running = false;
        gettimeofday(&tv_stop, NULL);
    }

    uint64_t get_time_ms()
    {
        uint64_t start_time_ms = (uint64_t) tv_start.tv_sec * 1000;
		start_time_ms +=  (uint64_t) (tv_start.tv_usec / 1000);

        uint64_t stop_time_ms = (uint64_t) tv_stop.tv_sec * 1000;
		stop_time_ms +=  (uint64_t) (tv_stop.tv_usec / 1000);

        uint64_t delta_time = stop_time_ms - start_time_ms;

        return delta_time;
    }

    void print_time_ms()
    {
        if(running)
        {
            Print::ln("Error stopwatch: trying to print delta time while running!");
            return;
        }

        Print::buf("Stopwatch delta: ");
        Print::ln(Str::UI(get_time_ms()));
    }

    void stop_and_print_ms()
    {
        stop();
        print_time_ms();
    }
};




struct PerformanceClock : public Clock
{
    enum Type
    {
        FPS,
        CommitBuffer,
        RenderCycle,
    };

    i64 fps_time_start_um = 0;
    i64 fps_time_end_um = 0;
    i64 commit_buffer_time_start_us = 0;

    PerformanceClock()
    {
        fps_start();
    }


    void start_commit_buffer_time()
    {
        commit_buffer_time_start_us = get_unix_epoch_us();
    }

    f32 get_commit_buffer_time_ms()
    {
        i64 commit_buffer_time_now_us = get_unix_epoch_us();
        i64 commit_buffer_time_delta_um = commit_buffer_time_now_us - commit_buffer_time_start_us;
        f32 commit_buffer_time_delta_ms = ((f32)commit_buffer_time_delta_um) * 0.001f;
        return commit_buffer_time_delta_ms;
    }

    void print_commit_buffer_time()
    {
        Print::ln(Str::FL(get_commit_buffer_time_ms(), 2, Str::FloatRep::Fixed));
    }


    void fps_start()
    {
        fps_time_end_um = 0;
        fps_time_start_um = get_unix_epoch_us();
    }
    void fps_end()
    {
        if(fps_time_start_um == 0)
        {
            Print::ln("WARNING: trying to end FPS-clock without setting a start fps time. ");
        }

        fps_time_end_um = get_unix_epoch_us();
    }
    f32 fps_get()
    {
        if(fps_time_start_um == 0)
            fps_end();

        i64 delta_time_us = fps_time_end_um - fps_time_start_um;

        f32 delta_time_ms = ((f32) delta_time_us) * 0.001f;

        return 1000.0f / delta_time_ms;
    }
    void fps_print()
    {
        Print::buf("FPS: ");
        Print::buf(Str::FL(fps_get(), 2, Str::FloatRep::Fixed));
        Print::buf("\n");
    }

    /** Pass 'true' as a parameter to print the value. This value defaults to false; */
    void fps_step(bool print = false)
    {
        fps_end();
        if(print)
            fps_print();
        fps_start();
    }

};