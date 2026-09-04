// #include <alsa/asoundlib.h>
// #include <stdio.h>
// #include <cstdint> // fixed width ints

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h> // memcpy

#include "lib/print.hh"
#include "lib/arr.hh"
// #include "lib/pair.hh"

#include "math/vec.hh"

#include "create_sine_buffer.hh"
#include "input.hh"
#include "DFT.hh"
#include "audio_data.hh"
#include "alsa.hh"
#include "wave_gen.hh"
#include "wav.hh"
#include "note.hh"
#include "instrument.hh"
#include "song.hh"
#include "sheet_io.hh"
#include "time.hh"
#include "phyano.hh"
#include "cli.hh"




void bin_dump(Str file_path, void* ptr, uint byte_count)
{
	int fd, ret;

	fd = open(file_path.to_c_str(), O_WRONLY | O_CREAT);
	if(fd < 0)
	{
		println("ERROR: Failed to open bin_dump-file for writing.");
		return;
	}
	ret = fchmod(fd, 00777);


	ret = write(fd, ptr, byte_count);
	if(ret < 0)
	{
		println("ERROR: Failed to write to bin_dump-file.");
		return;
	}

	ret = close(fd);
	if(ret < 0)
	{
		println("ERROR: Failed to close bin_dump-file.");
		return;
	}
}



void twinkle_twinkle()
{
	Alsa alsa;

	Song twinkle;
	twinkle.beat_count = 16;
	twinkle.bpm = 120;
	
	twinkle.notes[0].push_back({ NoteName::C4, NoteType::quarter});
	twinkle.notes[1].push_back({ NoteName::C4, NoteType::quarter});
	twinkle.notes[2].push_back({ NoteName::G4, NoteType::quarter});
	twinkle.notes[3].push_back({ NoteName::G4, NoteType::quarter});
	twinkle.notes[4].push_back({ NoteName::A4, NoteType::quarter});
	twinkle.notes[5].push_back({ NoteName::A4, NoteType::quarter});
	twinkle.notes[6].push_back({ NoteName::G4, NoteType::half	 });

	twinkle.notes[8].push_back({ NoteName::F4, NoteType::quarter});
	twinkle.notes[9].push_back( { NoteName::F4, NoteType::quarter});
	twinkle.notes[10].push_back( { NoteName::E4, NoteType::quarter});
	twinkle.notes[11].push_back( { NoteName::E4, NoteType::quarter});
	twinkle.notes[12].push_back( { NoteName::D4, NoteType::quarter});
	twinkle.notes[13].push_back( { NoteName::D4, NoteType::quarter});
	twinkle.notes[14].push_back( { NoteName::C4, NoteType::half   });

	twinkle.generate();
	twinkle.play(alsa);

	if(false)
	{
		WaveGen gen_C5(0.5, 523.3);
		FrequencyProfile profile_c5;
		profile_c5.generate_3_overtones(523.3);
		gen_C5.set_frequencies(profile_c5.frequencies);
		gen_C5.generate_wave();

		WaveGen gen_G5(0.5, 784.0);
		gen_G5.generate_wave();
		
		WaveGen gen_A5(0.5, 880.0);
		gen_A5.generate_wave();

		

		alsa.play(gen_C5.get_audio_data());
		alsa.play(gen_C5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
		alsa.play(gen_A5.get_audio_data());
		alsa.play(gen_A5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
	}

	// alsa.end();
}



void twinkle_json(Str file_path)
{
	// Print::buf("Song file: ");
	// Print::ln(sheet_file_path);
	
	// ResMove<Str> file_contents_mv = File::cat_as_str_core_xplat(file_path);
	// if(file_contents_mv.has_error())
	// {
	// 	Print::ln("Error: unable to read json sheet file.");
	// 	return;
	// }
	// Str file_contents = file_contents_mv.consume_value();

	Alsa alsa;

	Song twinkle_json;


	Str file_contents = File::try_cat(file_path);

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
		return;
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
	twinkle_json.beat_count = beat_count;
	twinkle_json.bpm = bpm;


	for(uint b_i = 0; b_i < beat_count; b_i++)
	{
		Str b_i_str = Str::UI(b_i);
		// Print::ln(b_i_str);

		OptPtr<Json> opt_b_i_array = beats_json.object_find(b_i_str);
		// Currently all beat entries has to be present in the beats-map/object
		if(opt_b_i_array.is_null())
		{
			Print::ln("ERROR: unable to find beat index array.");
			return;
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

			twinkle_json.notes[b_i].push_back(note);


			// Print
			// Print::buf(note_name_str);
			// Print::buf(", ");
			// Print::ln(note_type_str);
		}
	}

	twinkle_json.generate();
	twinkle_json.play(alsa);
	

	return;
}


void ambiance_song()
{
	Alsa alsa;

	Song ambiance;
	ambiance.beat_count = 4;
	ambiance.bpm = 60;

	ambiance.notes[0].push_back({ NoteName::C4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::D4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::E4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::F4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::G4, NoteType::half});

	ambiance.notes[1].push_back({ NoteName::C4, NoteType::half});

	ambiance.notes[2].push_back({ NoteName::C4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::D4, NoteType::half});
	// ambiance.notes[2].push_back({ NoteName::E4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::F4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::G4, NoteType::half});

	ambiance.notes[3].push_back({ NoteName::D4, NoteType::half});
	

	ambiance.generate();
	ambiance.play(alsa);

	// alsa.end();
}






int play_phyano()
{
	// CLOCK STUFF
	Clock clock;
	printf("Unix time ms: %li \n", clock.get_unix_epoch_ms());
	clock.print_tv_start();

	SleepTimer sleep_timer;
	// sleep_timer.sleep(1000);


	Phyano phyano;

	Arr<Key> keys;
	EvdevReader evdev_kbd {"/dev/input/event9"};

	// Start-up jingle
	sleep_timer.sleep(200);
	phyano.press(NoteName::C4); 
	sleep_timer.sleep(200);
	phyano.press(NoteName::G4); 

	for(int i = 0; i < 12000; i++)
	{
		sleep_timer.sleep(10); // poll 1000/X times / second

		keys = evdev_kbd.get_key_presses();
		// evdev_kbd.read_and_print(); // figure out keycodes for adding new keystrokes

		for(uint i = 0; i < keys.count(); i++)
		{
			switch (keys[i])
			{
				case Key::A:
					phyano.press(NoteName::C4); 
					break;
				case Key::S:
					phyano.press(NoteName::D4); 
					break;
				case Key::D:
					phyano.press(NoteName::E4); 
					break;
				case Key::F:
					phyano.press(NoteName::F4); 
					break;
				case Key::G:
					phyano.press(NoteName::G4); 
					break;
				case Key::H:
					phyano.press(NoteName::A4); 
					break;
				case Key::J:
					phyano.press(NoteName::B4); 
					break;
				case Key::K:
					phyano.press(NoteName::C5); 
					break;

				case Key::ESC:
					Print::ln("Escape key pressed. Exiting Phyano.");

					// Shut-down jingle
					sleep_timer.sleep(200);
					phyano.press(NoteName::G4); 
					sleep_timer.sleep(350);
					phyano.press(NoteName::C4); 

					return 0; 

					break;
				
				default:
					break;
			}
		}

		keys.clear();
		
	}

	return 0;
}

int sheet_command(Str flag1, Str flag2)
{
	if(flag1 == "play")
	{
		Alsa alsa;
		Song song;

		song = SheetIO::load_song(flag2);

		song.generate();
		song.play(alsa);

	}
	if(flag1 == "export")
	{
		Song song;
		song.set_beat_count(8);
		// song.beat_count = 8;
		song.bpm = 90;

		Note c4 {NoteName::C4, NoteType::quarter}; 
		Note d4 {NoteName::D4, NoteType::quarter}; 
		Note e4 {NoteName::E4, NoteType::quarter}; 

		song.notes[0].push_back(c4);
		song.notes[1].push_back(d4);
		song.notes[2].push_back(e4);
		song.notes[3].push_back(c4);
		song.notes[4].push_back(d4);
		song.notes[5].push_back(e4);

		song.notes[7].push_back(c4);

		SheetIO::export_song(song, "tmp/sheet.json");

	}

	return 0;
}



int DFT_tests()
{
	#include "math/const.hh"

	std::complex<double> a = 1;
	std::complex<double> b = 2.0 - 1.0i;
	std::complex<double> c = -1.0i;
	std::complex<double> d = -1.0 + 2.0i;

	Arr<std::complex<double>> input_arr;
	input_arr.push_back(a);
	input_arr.push_back(b);
	input_arr.push_back(c);
	input_arr.push_back(d);
	Vec<std::complex<double>> input_vec {4};
	input_vec[0]= a;
	input_vec[1]= b;
	input_vec[2]= c;
	input_vec[3]= d;

	// Soinusoid
	uint sample_count = 25;
	double sample_count_db = (double) sample_count;
	double t0 = 0; // Starting time
	double tf = 24.0; // End Time
	double Dt = (tf - t0); // Total temporal duration of all samples
	double dt = Dt / ((double) (sample_count-1)); // dime between samples
	double Fs = 1 / dt; // Sampling frequency : samples / sec

	Vec<double> T;
	T.set_size(sample_count);
	Vec<double> F;
	F.set_size(sample_count);

	for(uint i = 0; i < sample_count; i++)
	{
		double i_d = (double) i;
		double t = i_d * dt;
		T[i] = t;
		
		// The BELOW div(index, total_time) is not the confirmed correct way to get the frequencies!
		// if(i > sample_count / 2)
		// 	F[i] = (double) (sample_count - i) / Dt;
		// else
		// 	F[i] = (double) i / Dt;
		if(i > sample_count / 2)
			F[i] = (double) (sample_count - i) * Fs / sample_count_db;
		else
			F[i] = (double) i * Fs / sample_count_db;
	}
	// print_vec(T);
	// print_vec(F);

	Vec<std::complex<double>> sinusoid { sample_count };
	// std::complex<double> ampl = 10.0;
	// std::complex<double> freq = 2.0;

	
	
	double i_d;
	for(uint i = 0; i < sample_count; i++)
	{
		i_d = (double) i;
		double t = i_d * dt;
		// sinusoid[i] = ampl * std::sin( 2 * 3.1415 * freq * (i_d * dt) );
		// sinusoid[i] = 2.0 * sin( PI2 *  t / 25.0 ) + cos(PI2 * 4.0 * t / 25.0);
		sinusoid[i] = cos(PI2 * t * (0.400)) + 2.0 * sin( PI2 *  t * (0.320) );
	}

	print_complex_vec(sinusoid);

	// Arr<std::complex<double>> output = DFT::calculate(input_arr);
	// Vec<std::complex<double>> output = DFT::calculate(input_vec);
	Vec<std::complex<double>> output = DFT::calculate(sinusoid);

	print_complex_vec(output);

	Vec<double> mag_vec = complex_vec_to_mag_vec(output);
	Vec<double> real_vec = complex_vec_to_real_vec(output);
	Vec<double> imag_vec = complex_vec_to_imag_vec(output);
	Vec<double> freq_vec; // frequencies associated with the DFT output

	print_vec(complex_vec_to_mag_vec(output));

	Arr<Vec<double>> arr_vecs;
	// arr_vecs.reserve(10);

	arr_vecs.push_back(F);
	arr_vecs.push_back(real_vec);
	arr_vecs.push_back(imag_vec);

	// vec_vec.set_size(2);

	// vec_vec[0] = mag_vec;
	// vec_vec[1] = mag_vec;
	Arr<Str> col_names;
	col_names.push_back("Freq");
	col_names.push_back("Cos/real");
	col_names.push_back("Sin/imag");

	print_vecs(arr_vecs, col_names);



	

	// for(uint i = 0; i < output.count(); i++)
	// for(uint i = 0; i < output.size(); i++)
	// {
	// 	print(Str::FL(output[i].real(), 3, Str::FloatRep::Fixed));
	// 	print(" + ");
	// 	print(Str::FL(output[i].imag(), 3, Str::FloatRep::Fixed));
	// 	print(" i \n");
	// }


	return 0;
}



int main_2(int argc, char** argv)
{
    println("Physimos::audio starting!");


	// twinkle_twinkle();
	ambiance_song();


    printf("End Alsa test\n");

	return 0;
}


int main(int argc, char** argv)
{
    println("Physimos::audio starting!");

	CLI cli (argc, argv);
	// cli.print();


	if(cli[1] == "phyano")
	{
		return play_phyano();
	}
	else if(cli[1] == "sheet")
	{
		return sheet_command(cli[2], cli[3]);
	}
	else if(cli[1] == "DFT")
	{
		return DFT_tests();
	}



	// ambiance_song();


	// alsa0.end();    

	// while(1) {}



	// SineWave sine_wave {1.0};
	Arr<Frequency> frequencies;
	// frequencies.push_back({3520.0, 0.05});
	// frequencies.push_back({1760.0, 0.1});
	// frequencies.push_back({880.0, 0.5});
	// frequencies.push_back({440.0, 0.15});
	// frequencies.push_back({220.0, 0.10});
	// frequencies.push_back({110.0, 0.10});

	frequencies.push_back({880.0, 	0.15});
	// frequencies.push_back({660.0, 	0.15});
	// frequencies.push_back({440.0, 	0.20});
	// frequencies.push_back({220.0, 	0.70});

	// frequencies.push_back(900.0);
	// frequencies.push_back(800.0);
	// frequencies.push_back(700.0);
	// frequencies.push_back(600.0);
	// frequencies.push_back({560.0, 0.02});
	// frequencies.push_back({760.0, 0.02});
	// frequencies.push_back({80.0, 0.15});

	WaveConfig w_config;
	WaveGen wave_generator {1.0, frequencies};
	// sine_wave.set_config(default_wave_config);
	// sine_wave.w_config.gain = 0.2;
	wave_generator.generate_wave();
	// sine_wave.print_wave();

	WAV wav; 
	// wav.populate_from_wave(sine_wave);
	// wav.to_little_endian();
	// wav.write_to_file("tmp/test2.wav");
	wav.read_file("resources/audio/sine.wav");
	wav.write_to_file("tmp/sine_out.wav");
	print("Reading Wav file. Size = ");
	print(Str::CH(wav.data_chunk.DataBlocID[1]));
	println(Str::SI(wav.data_chunk.DataSize));
	bin_dump("tmp/dump.bin", &wav, 44);

	// Print sizes of Wav structs
	println(Str::SI(sizeof(WavHeaderRIFF)));
	println(Str::SI(sizeof(WavHeaderDataFormat)));
	println(Str::SI(sizeof(WavDataChunk)));

	println(Str::SI(sizeof(WAV)));


    // Arr<int> sine_buff = create_sine_buffer();
	// int* sine_buff_ptr = sine_buff.data_mut();

    // print buff
    // for(uint i = 0; i < sine_buff.count(); i++)
    // {
    //     print(Str::SI(sine_buff[i]));
    //     print(" ");
    // }
	// print("\n");

	
	unsigned int rate, channels;

	if (argc > 2)
    {
		// printf("Usage: %s <sample_rate> <channels> <seconds>\n", argv[0]);
		// return -1;
		rate 	 = atoi(argv[1]);
		channels = atoi(argv[2]);
	}
	else
	{
		rate 	 = 44100;
		channels = 1;
	}


	WaveGen wave_100hz(0.5, 300.0);
	wave_100hz.generate_wave();

	WaveGen wave_1000hz(0.5, 1000.0);
	wave_1000hz.generate_wave();

	

	// Alsa alsa { 
	// 	rate, 
	// 	channels,
	// 	"resources/audio/sine.wav"
	// };
	Alsa alsa;

	AudioData audio_data;
	audio_data.data = wave_generator.out_arr;

	alsa.print_pcm_info();
	// alsa.play();
	// alsa.play(audio_data);
	// alsa.play(wave_100hz.get_audio_data());
	// alsa.play(wave_1000hz.get_audio_data());
	// alsa.end();

	// twinkle_twinkle();
	ambiance_song();


    printf("End Alsa test\n");

	return 0;
}


