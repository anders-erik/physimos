#pragma once

#include "lib/arr.hh"
#include "alsa.hh"
#include "note.hh"
#include "audio_data.hh"


class Song
{
public:

	double bpm = 120.0; // beat / minute
	uint beat_count = 16;

	Arr<Arr<Note>> notes;
	AudioData song_data;

	// Arr<Pair<Note, AudioData>> notes_data;
	// Arr<Note> notes;

	Song() : notes {beat_count, {}}
	{
	}

	void set_beat_count(uint _beat_count)
	{
		beat_count = _beat_count;

		notes.clear();
		notes.set({}, beat_count);
	}


	void add_wave_to_audiodata_at_beat_count(AudioData& _audio_data, uint beat_index, double beat_note_gain/*gain for specific note during the current beat index*/)
	{

		uint samples_per_beat = (uint)(44100.0 * (60.0 / bpm));

		uint first_sample_offset = samples_per_beat * beat_index;
		// uint last_sample_offset = first_sample_offset + _audio_data.sample_count();

		for(uint i = 0; i < _audio_data.sample_count(); i++)
		{
			int16_t wave_data_with_gain_adjusted = (int16_t) (beat_note_gain * (double)_audio_data.data[i]);
			song_data.data[first_sample_offset + i] += wave_data_with_gain_adjusted;
		}
	}

	void generate()
	{	
		double song_duration_s = (double)beat_count * 60.0 / bpm;
		double sample_count = 44100 * song_duration_s;

		song_data.set_sample_count(sample_count);

		for(uint beat_i = 0; beat_i < notes.count(); beat_i++)
		{

			// double beat_note_gain = 1.0 / sqrt( (double) notes[beat_i].count() ) ; // reduce frequency amplitude when multiple keys are pressed at the same beat
			double beat_note_gain = 0.6 / sqrt( (double) notes[beat_i].count() ) ; // sqrt is good, only need default single key gain be < 1.0
			// double beat_note_gain = 1.0 / (double) notes[beat_i].count();
			// double beat_note_gain = 0.1;

			for(uint note_i = 0; note_i < notes[beat_i].count(); note_i++)
			{
				AudioData note_data = Instrument::get_note_audio(notes[beat_i][note_i], bpm, 0.5);
				add_wave_to_audiodata_at_beat_count(note_data, beat_i, beat_note_gain);
			}
		}
	}

	void play(Alsa& alsa)
	{
		alsa.play(song_data);

		// for(uint i = 0; i < notes.count(); i++)
		// {
			// Note note = notes[i];

			// // Find the already generated audio data for the note
			// for(uint j = 0; j < notes_data.count(); j++)
			// {
			// 	if(note == notes_data[j].XX)
			// 	{
			// 		alsa.play(notes_data[j].YY);
			// 		break;
			// 	}
			// }
		// }
	}
};