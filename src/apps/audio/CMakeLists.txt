


add_executable(main_audio main_audio.cc)
target_link_libraries(main_audio
    lib
    audio
    asound
    io
    alsa
)

include_directories(main_audio
    .
    ../..
)
