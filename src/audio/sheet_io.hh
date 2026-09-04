#pragma once



#include "io/json/json.hh"
#include "lib/file.hh"

class SheetIO
{
public:

	static void export_song(Song& _song, Str _file_path)
	{
		Json json;
		json.set_object();
		json.object_push({"format", j_string{"physimos.sheet"} });
		json.object_push({"version", j_string{"0"}});
		json.object_push({"name", j_string{"random"}});
		json.object_push({"instrument", j_string{"pure"}});
		json.object_push({"beat_count", j_int{8} });
		json.object_push({"bpm", j_int{90} });
		// json.object_push({"beats", j_object{}});

		Json beats;
		beats.set_object();

		// Add the beat-arrays
		for(int i = 0; i < _song.beat_count; i++)
		{
			
		}


		for(uint beat_i = 0; beat_i < _song.notes.count(); beat_i++)
		{
			Json beat_array;
			beat_array.set_array();

			for(uint note_i = 0; note_i < _song.notes[beat_i].count(); note_i++)
			{
				Note note = _song.notes[beat_i][note_i];
				Json jnote;
				// Json note_name
				jnote.set_object();
				jnote.object_push({"note", NoteSerializer::note_name_to_str(note.name)});
				jnote.object_push({"type", NoteSerializer::note_type_to_str(note.type)});

				beat_array.get_array().push_back(jnote);
			}

			beats.object_push({Str::SI(beat_i), beat_array});
		}

		json.object_push({"beats", beats});

		Str json_str = json.try_serialize();

		int fd = open(_file_path.to_c_str(), O_WRONLY | O_CREAT);
		fchmod(fd, 0777);
		if(fd < 0)
		{
			Print::ln("Error: failed to open file to write json object in SheetIO.");
			return;
		}
		
		write(fd, json_str.to_c_str(), json_str.size());

		close(fd);

		return;
	}

	static Song load_song(Str _file_path)
	{
		Song song;

		Str file_contents = File::try_cat(_file_path);

		Json json;
		json.try_parse(file_contents);


		// READ FORMAT
		OptPtr<Json> opt_format = json.object_find("format");
		OptPtr<Json> opt_version = json.object_find("version");
		OptPtr<Json> opt_name = json.object_find("name");
		OptPtr<Json> opt_instrument = json.object_find("instrument");
		OptPtr<Json> opt_beat_count = json.object_find("beat_count");
		OptPtr<Json> opt_bpm = json.object_find("bpm");
		OptPtr<Json> opt_beats = json.object_find("beats");

		if(	opt_format.is_null() 		|| 
			opt_version.is_null() 		|| 
			opt_name.is_null() 			|| 
			opt_instrument.is_null() 	|| 
			opt_beat_count.is_null() 	|| 
			opt_bpm.is_null() 			|| 
			opt_beats.is_null()
		)
		{
			Print::ln("Error: unable to parse json format in twinkle.sheet.json.");
			return Song{};
		}

		// All objects were found. Extract the values from the Json containers
		Str format = opt_format.get_ref().get_string();
		Str version = opt_version.get_ref().get_string();
		Str name = opt_name.get_ref().get_string();
		Str instrument = opt_instrument.get_ref().get_string();
		uint beat_count = (uint) opt_beat_count.get_ref().get_int();
		uint bpm =  (uint) opt_bpm.get_ref().get_int();
		Json beats_json = opt_beats.get_ref();

		// PRINT FORMAT
		if(false)
		{
			Print::ln(format);
			Print::ln(version);
			Print::ln(name);
			Print::ln(instrument);
			Print::ln(Str::SI(beat_count));
			Print::ln(Str::SI(bpm));
		}

		// Update actual song data
		song.set_beat_count(beat_count);
		song.bpm = bpm;


		for(uint b_i = 0; b_i < beat_count; b_i++)
		{
			Str b_i_str = Str::UI(b_i);
			// Print::ln(b_i_str);

			OptPtr<Json> opt_b_i_array = beats_json.object_find(b_i_str);
			// Currently all beat entries has to be present in the beats-map/object
			if(opt_b_i_array.is_null())
			{
				Print::ln("ERROR: unable to find beat index array.");
				return Song{};
			}

			j_array b_i_array = opt_b_i_array.get_ref().get_array();

			for(size_t a_i = 0; a_i < b_i_array.size(); a_i++)
			{
				Json note_obj = b_i_array[a_i];
				Str note_name_str = note_obj.object_try_find("note").get_string();
				Str note_type_str = note_obj.object_try_find("type").get_string();

				Note note;

				if(note_type_str == "quarter")
					note.type = NoteType::quarter;
				else if(note_type_str == "half")
					note.type = NoteType::half;
				else if(note_type_str == "whole")
					note.type = NoteType::whole;

				if(note_name_str == "C4")
					note.name = NoteName::C4;
				else if(note_name_str == "D4")
					note.name = NoteName::D4;
				else if(note_name_str == "E4")
					note.name = NoteName::E4;
				else if(note_name_str == "F4")
					note.name = NoteName::F4;
				else if(note_name_str == "G4")
					note.name = NoteName::G4;
				else if(note_name_str == "A4")
					note.name = NoteName::A4;
				else if(note_name_str == "B4")
					note.name = NoteName::B4;
				else if(note_name_str == "C5")
					note.name = NoteName::C5;

				song.notes[b_i].push_back(note);


				// Print
				// Print::buf(note_name_str);
				// Print::buf(", ");
				// Print::ln(note_type_str);
			}
		}

		return song;
	}

	

};