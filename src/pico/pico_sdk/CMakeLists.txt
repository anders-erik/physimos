

# CHATGPT PICO

cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(my_pico_project C CXX ASM)

pico_sdk_init()

add_executable(main_pico_sdk
    main_pico_sdk.cc
    usb_descriptors.c
)

# target_include_directories(main_pico_sdk
#     ${CMAKE_CURRENT_LIST_DIR}/pico-sdk/lib/tinyusb/src
# )
target_link_libraries(main_pico_sdk 
    pico_stdlib
    tinyusb_device
)

# tusb_config.h lives next to this CMakeLists.txt; tinyusb includes it with
# quotes relative to its own source directory, so it must be on the include path.
target_include_directories(main_pico_sdk PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Disabled: pico's canned USB-CDC stdio driver would conflict with our own
# raw TinyUSB HID device (both would try to own the USB stack/descriptors).
pico_enable_stdio_usb(main_pico_sdk 0)
pico_enable_stdio_uart(main_pico_sdk 0)

pico_add_extra_outputs(main_pico_sdk)

