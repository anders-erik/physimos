#pragma once

#include <chrono>
#include <sys/time.h>
#include <unistd.h>

#include "lib/clock.hh"
#include "print.hh"

struct Time
{
    static constexpr void print_current_time()
    {
        // std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        // Print::ln(Str::UI(tp));
    }


};

struct Timer
{
    std::chrono::system_clock::time_point start_tp;
    std::chrono::system_clock::time_point stop_tp;
    std::chrono::nanoseconds duration_ns;

    std::chrono::_V2::system_clock::rep reps;

    void start(){
        start_tp = std::chrono::system_clock::now();
    }
    void stop(){
        stop_tp = std::chrono::system_clock::now();

        duration_ns = stop_tp - start_tp;
        reps = duration_ns.count();
    }
    std::chrono::nanoseconds get_duration(){
        return duration_ns;
    }
    auto& get_reps()
    {
        return reps;
    }
    double get_micros()
    {
        return (double) (reps / 1000);
    }
    float get_ms()
    {
        return (float) (reps / 1000000);
    }
    /** Seconds elapsed between start and stop. */
    float get_s()
    {
        // double db_ms = (double) get_ms();
        return (float) (get_micros() / 1000000);
    }
    void print_duration(Str str_msg){
        // str_msg.print();
        print(str_msg);
        Print::ln(Str::UI(reps));
        // std::cout << " " << reps << std::endl;
    }
    void print_ms(Str str_msg){
        // str_msg.print();
        print(str_msg);
        Print::ln(Str::FL(get_ms(), 1, Str::FloatRep::Fixed));
        // std::cout << " " << reps << std::endl;
    }
    void print_s(Str str_msg){
        print(str_msg);
        Print::ln(Str::FL(get_s(), 4, Str::FloatRep::Fixed));
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