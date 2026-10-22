#pragma once

#include "lib/arr.hh"
#include "lib/llist.hh"
#include "lib/pair.hh"

#include "alsa.hh"
#include "audio_data.hh"
#include "instrument.hh"
#include "note.hh"
#include "audio/wave_generator.hh"


/* Specifying an arbitrary time at which a note is played in the instrument line */
struct LineNote
{
	// i32 start_time_ms;
	i32 start_index_16th_count; // the n-th sixteenth note-duration on which this note begins
	Note note;
	f64 bpm = 120.0;
	NoteType time_step_type = NoteType::sixteenth;

	LineNote() {}
	LineNote(i32 _start_index_16th_count, Note _note)
		: 	start_index_16th_count {_start_index_16th_count},
			note {_note}
	{

	}

	f64 get_start_time_s()
	{
		return ( ((f64)start_index_16th_count) * NoteTypeDuration::get_duration_s(time_step_type, bpm));
	}

	f64 get_start_time_ms()
	{
		return get_start_time_s() * 1000.0;
	}

	void set_start_time_ms(f64 _start_time_ms)
	{
		f64 time_step_ms = NoteTypeDuration::get_duration_s(time_step_type, bpm) * 1000.0;

		start_index_16th_count = (i32) (_start_time_ms / time_step_ms);
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
	// Arr<LineNote> line_notes;
	LList<LineNote> line_notes;
	f64 bpm = 120;
	AudioData32 line_audio_data;

	LineNote* new_note()
	{
		LLNode<LineNote>* new_node = line_notes.append();
		return &(new_node->value);
	}
	// void add_note(LineNote _line_note)
	// {
	// 	// line_notes.push_back(_line_note);
	// 	LLNode<LineNote>* new_node = line_notes.append();
	// }

	void set_sampling_config(AudioSamplingConfig _sampling_config)
	{
		line_audio_data.set_sampling_config(_sampling_config);

	}


	void generate()
	{

		for(uint i = 0; i < line_notes.count(); i++)
		{
			LineNote* line_note = line_notes[i];

			AudioData32 note_audio_data = SineWave::generate_damped_wave(	line_audio_data.sampling_config, 
																			NoteFrequencies::to_frequency(line_note->note.name)	);

			u32 index_offset = line_note->get_starting_sample_index(line_audio_data.sampling_config);

			for(uint i = 0; i < note_audio_data.sample_count(); i++)
				line_audio_data.data[index_offset + i] += note_audio_data.data[i];
		}

	}
};


class Song
{
public:

	double bpm = 120.0; // beat / minute
	AudioSamplingConfig sampling_config;

	AudioData16 song_data;

	// Arr<InstrumentLine> instrument_lines;
	InstrumentLine piano_line;


	Song()
	{
	}


	void set_sampling_config(AudioSamplingConfig _sampling_config)
	{
		sampling_config = _sampling_config;
		piano_line.set_sampling_config(sampling_config);
	}


	void add_wave_to_audiodata_at_time(AudioData16& _audio_data, double _time_ms, double beat_note_gain/*gain for specific note during the current beat index*/)
	{
		if( (_audio_data.duration_s_double() + _audio_data.duration_s_double()) > sampling_config.get_duration_s())
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
		song_data.set_sample_count(sampling_config.sample_count);

		// PIANO LINE
		for(uint i = 0; i < piano_line.line_notes.count(); i++)
		{
			LineNote* line_note = piano_line.line_notes[i];
			// AudioData16 note_data = Instrument::get_note_audio(line_note.note, bpm, 0.2);

			AudioSamplingConfig sampling_config_note {	AudioSamplingConfig::RATE_DURATION, 
														44100, 
														NoteTypeDuration::get_duration_us(line_note->note.type, this->bpm) };

			// AudioData32 note_data_32 = SineWave::generate_damped_wave(	sampling_config_note, 
			// 															NoteFrequencies::to_frequency(line_note->note.name));
			// AudioData16 note_data_16 = note_data_32.to_audio_data_i16();

			AudioData16 note_data_16 = PianoWave::generate(	sampling_config_note, 
															NoteFrequencies::to_frequency(line_note->note.name));


			add_wave_to_audiodata_at_time(note_data_16, line_note->get_start_time_ms(), 1.0);
		}
	}

	void play(Alsa& alsa)
	{
		alsa.play(song_data);
	}

	void stop(Alsa& alsa)
	{
		alsa.stop();
	}
};

