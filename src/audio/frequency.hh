#pragma once

/** Represents a specific frequency and an amplitude value to indicte its strength */
struct AudioFrequency
{
	double frequency; // oscillations per second
	double amp; // [0, 1]
};
