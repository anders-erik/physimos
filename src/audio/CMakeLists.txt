
# C
set(CMAKE_C_COMPILER gcc)

# add_executable(main_audio_c main_audio.c)
# target_link_libraries(main_audio_c 
#     asound
# )

unset(CMAKE_C_COMPILER)


add_library(alsa alsa.cc)
target_link_libraries(alsa 
    lib
    asound
    io
)

add_library(audio
    audio.cc
    wave_generator.cc
)
target_link_libraries(audio 
    lib
    alsa
    io
)

# CPP
add_executable(main_audio main_audio.cc)
target_link_libraries(main_audio
    lib
    audio
    asound
    io
    alsa
)