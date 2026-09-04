#pragma once

#include "alsa.hh"
#include "note.hh"
#include "instrument.hh"
#include "audio_data.hh"


class Phyano
{
public:

	// Alsa object were intended to be placed in a container, but the alsa object did not comply.
	// Should be resolved in the near future.
	// Should be trivial with proper move sematins in place for Alsa. Currently copy construction is disabled.
	// Arr<Alsa> alsas;

	Alsa alsa_0;
	Alsa alsa_1;
	Alsa alsa_2;
	Alsa alsa_3;
	Alsa alsa_4;
	Alsa alsa_5;
	Alsa alsa_6;
	Alsa alsa_7;

	uint available_alsa_count = 8;
	uint alsa_index = 0;

	AudioData adata; // 1 second databuffer

	// Phyano() : alsas {10}
	Phyano()
	{
		adata.set_sample_count(44100); // 1 sec

		
		for(uint i = 0; i < available_alsa_count; i++)
		{
			// Alsa alsa_tmp;
			// alsas.push_back(alsa_tmp);
			// alsas.emplace_back({});
		}
		// alsas.expand();
		// alsas.set({}); 
	}

	void press(NoteName _note_name)
	{
		AudioData note_data = Instrument::get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.3);

		if(++alsa_index >= available_alsa_count)
			alsa_index = 0;

		switch (alsa_index)
		{
			case 0:
				alsa_0.play(note_data);
				break;
			case 1:
				alsa_1.play(note_data);
				break;
			case 2:
				alsa_2.play(note_data);
				break;
			case 3:
				alsa_3.play(note_data);
				break;
			case 4:
				alsa_4.play(note_data);
				break;
			case 5:
				alsa_5.play(note_data);
				break;
			case 6:
				alsa_6.play(note_data);
				break;
			case 7:
				alsa_7.play(note_data);
				break;
		
		default:
			break;
		}

		// alsa.play(note_data);
	}

	void add_press(Instrument _instument, NoteName _note_name)
	{
		AudioData note_data = _instument.get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.5);

		if(note_data.sample_count() > 44100)
		{
			println("ERROR: trying to play Phyano note longer than buffer size.");
			return;
		}

		for(uint i = 0; i < note_data.sample_count(); i++)
		{
			adata.data[i] += note_data.data[i];
		}
	}

	AudioData consume_ms(uint ms)
	{
		if(ms > 1000)
		{
			println("ERROR: can't consume more than 1000 ms of Phyano data.");
			return AudioData {};
		}

		uint samples_per_ms = 44;
		uint samples_to_consume = ms * samples_per_ms;

		AudioData return_adata;
		return_adata.set_sample_count(samples_to_consume);

		for(uint i = 0; i < return_adata.sample_count(); i++)
		{
			return_adata.data[i] += adata.data[i];
		}

		shift_data(samples_to_consume);

		return return_adata;
	}


	void shift_data(uint shift_count)
	{
		// Copy
		for(uint i = shift_count; i < adata.sample_count(); i++)
		{
			adata.data[i-shift_count] = adata.data[i];
		}

		// Clear data tail
		for(uint i = (adata.sample_count() - shift_count); i < adata.sample_count(); i++)
		{
			adata.data[i] = 0;
		}
	}

};