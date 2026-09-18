

# CHATGPT PICO

cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(my_pico_project C CXX ASM)

pico_sdk_init()

add_executable(main_pico_sdk
    main_pico_sdk.cc
)
target_link_libraries(main_pico_sdk 
    pico_stdlib
)

# target_include_directories(main_pico_sdk
#     pico-sdk/src/common/pico_stdlib_headers/include
# )
# src/pico/pico_sdk/pico-sdk/src/common/pico_stdlib_headers

pico_enable_stdio_usb(main_pico_sdk 1)
pico_enable_stdio_uart(main_pico_sdk 0)

pico_add_extra_outputs(main_pico_sdk)

