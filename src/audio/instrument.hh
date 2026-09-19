#pragma once

#include "audio_data.hh"
#include "note.hh"
#include "wave_generator.hh"

class Instrument
{
public:

	static AudioData get_note_audio(Note note, double tempo_bpm, double _gain)
	{
		AudioWaveGenerator wave_generator;

		// Duration
		double duration = 1.0 / (tempo_bpm / 60.0); // seconds / beat (for quarter note)

		if(note.type == NoteType::whole)
			duration *= 4.0; // four beats
		else if(note.type == NoteType::half)
			duration *= 2.0; // two beats
		else if(note.type == NoteType::quarter)
			duration *= 1.0; // one beats
		
		// Frequency
		wave_generator.wave_freqs.clear();

		if(note.name == NoteName::C4)
			wave_generator.wave_freqs.push_back({261.63, 1.0});
		else if(note.name == NoteName::D4)
			wave_generator.wave_freqs.push_back({293.66, 1.0});
		else if(note.name == NoteName::E4)
			wave_generator.wave_freqs.push_back({329.63, 1.0});
		else if(note.name == NoteName::F4)
			wave_generator.wave_freqs.push_back({349.23, 1.0});
		else if(note.name == NoteName::G4)
			wave_generator.wave_freqs.push_back({392.0, 1.0});
		else if(note.name == NoteName::A4)
			wave_generator.wave_freqs.push_back({440.0, 1.0});
		else if(note.name == NoteName::B4)
			wave_generator.wave_freqs.push_back({493.88, 1.0});
		else if(note.name == NoteName::C5)
			wave_generator.wave_freqs.push_back({523.25, 1.0});
		
		wave_generator.config.set_duration(duration);
		wave_generator.config.set_gain(_gain);

		wave_generator.generate_wave();

		return wave_generator.out_arr;
	}
};