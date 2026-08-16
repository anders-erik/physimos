
# Audio

Audio library and CLI tool for generating and playing audio.


## Dependencies

### Utilities

- ffmpeg

### ALSA
- libasound2-dev

### PulseAudio


## TODO

- Play wav file using alsa
- Generate wav file in main_audio

## Run

`$ ./main_audio_c 44100 1 1 < /ae/dev/physimos/resources/audio/M1F1-Alaw-AFsp.wav`
`$ ./main_audio_c 8000 2 2 < /ae/dev/physimos/resources/audio/M1F1-Alaw-AFsp.wav`

## Terminal Commands

`$ ffprobe <audio-file.ext>`
`$ ./build/debug_linux/src/audio/main_audio 48000 1 1 < /ae/dev/physimos/resources/audio/sine.wav` // set freq - channels - duration
`$ ./build/debug_linux/src/audio/main_audio < /ae/dev/physimos/resources/audio/sine.wav` // default freq - channels - duration

