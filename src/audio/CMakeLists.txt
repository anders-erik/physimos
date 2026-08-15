
# C
set(CMAKE_C_COMPILER gcc)

add_executable(main_audio_c main_audio.c)
target_link_libraries(main_audio_c 
    asound
)

unset(CMAKE_C_COMPILER)


# CPP
add_executable(main_audio main_audio.cc)
target_link_libraries(main_audio
    lib
    asound
)