#pragma once

#include "lib/arr.hh"
#include "lib/pair.hh"

#include "alsa.hh"
#include "audio_data.hh"
#include "instrument.hh"
#include "note.hh"


/* Specifying an arbitrary time at which a note is played in the instrument line */
struct LineNote
{
	i32 start_time_ms;
	Note note;

	f64 get_start_time_s()
	{
		return ((f64)start_time_ms) / 1000;
	}

	u32 get_starting_sample_index(AudioSamplingConfig _sampling_config)
	{
		return (u32) ((f64)_sampling_config.sample_rate_s * get_start_time_s());
		// double first_sample_offset_db = (double)_sampling_config.sample_rate_s * get_start_time_s();
		// uint first_sample_offset = (uint) first_sample_offset_db;
	}	
};


struct InstrumentLine
{
	Instrument instrument;
	Str name;
	double gain = 0.5;
	Arr<LineNote> line_notes;
	AudioData32 line_audio_data;

	void add_note(LineNote _line_note)
	{
		line_notes.push_back(_line_note);
	}

	void set_sampling_config(AudioSamplingConfig _sampling_config)
	{
		line_audio_data.set_sampling_config(_sampling_config);
	}


	void generate()
	{

		for(uint i = 0; i < line_notes.count(); i++)
		{
			LineNote line_note = line_notes[i];

			AudioData32 note_audio_data = SineWave::generate_damped_wave(	line_audio_data.sampling_config, 
																			NoteFrequencies::to_frequency(line_note.note.name)	);

			u32 index_offset = line_note.get_starting_sample_index(line_audio_data.sampling_config);

			for(uint i = 0; i < note_audio_data.sample_count(); i++)
				line_audio_data.data[index_offset + i] += note_audio_data.data[i];
		}

	}
};


class Song
{
public:

	double bpm = 120.0; // beat / minute
	uint beat_count = 8;

	Arr<Arr<Note>> notes;
	AudioData16 song_data;

	// Arr<InstrumentLine> instrument_lines;
	InstrumentLine piano_line;

	// Arr<Pair<Note, AudioData>> notes_data;
	// Arr<Note> notes;

	Song() : notes {beat_count, {}}
	{
	}

	void set_beat_count(uint _beat_count)
	{
		if(beat_count == _beat_count)
			return;

		beat_count = _beat_count;

		notes.clear();
		notes.set({}, beat_count);
	}

	void set_tempo_bmp(double _bpm)
	{
		bpm = _bpm;
	}

	double song_length_s()
	{
		double seconds_per_beat = (1 / bpm) * 60.0;
		return seconds_per_beat * (double) beat_count;
	}

	void add_wave_to_audiodata_at_beat_count(AudioData16& _audio_data, uint beat_index, double beat_note_gain/*gain for specific note during the current beat index*/)
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

	void add_wave_to_audiodata_at_time(AudioData16& _audio_data, double _time_ms, double beat_note_gain/*gain for specific note during the current beat index*/)
	{
		if( (_audio_data.duration_s_double() + _audio_data.duration_s_double()) > song_length_s())
		{
			Print::ln("Added note extensds beyond the length of the song. Note note added!");
			return;
		}

		uint first_sample_offset = song_data.sample_rate() * _time_ms / 1000;
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
				AudioData16 note_data = Instrument::get_note_audio(notes[beat_i][note_i], bpm, 0.5);
				add_wave_to_audiodata_at_beat_count(note_data, beat_i, beat_note_gain);
			}
		}

		// PIANO LINE
		for(uint i = 0; i < piano_line.line_notes.count(); i++)
		{
			LineNote line_note = piano_line.line_notes[i];
			AudioData16 note_data = Instrument::get_note_audio(line_note.note, bpm, 0.2);
			add_wave_to_audiodata_at_time(note_data, line_note.start_time_ms, 0.5);
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

	void stop(Alsa& alsa)
	{
		alsa.stop();
		// TODO: Figure out how to stop alsa from reading the data we have already written
	}
};
