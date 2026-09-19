#pragma once

#include "lib/arr.hh"

#include "frequency.hh"


/** Generates a frequency domain profile based on input functions. Purpose is to experiment with resulting timbres when tranformed into time domain.  */
struct FrequencyProfile
{
	Arr<AudioFrequency> frequencies;

	void generate_3_overtones(double base_frequency)
	{
		Arr<AudioFrequency> freq_tmp;

		freq_tmp.push_back({base_frequency * 1.0, 1.00});
		freq_tmp.push_back({base_frequency * 2.0, 0.30});
		freq_tmp.push_back({base_frequency * 3.0, 0.10});
		// freq_tmp.push_back({base_frequency * 4.0, 0.10});

		frequencies = freq_tmp;
	}
};