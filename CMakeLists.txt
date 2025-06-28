# Top-level CMakeLists.txt

# Run commands
# $ mkdir build_cmake && cd build_cmake
# $ rm -r ./** && cmake .. && make && cd .. && ./build_cmake/physimos && cd ./build_cmake


message("CMake version is " ${CMAKE_VERSION})
cmake_minimum_required(VERSION 3.28)

project(physimos)

set(CMAKE_CXX_STANDARD 23)


# set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)
# write_file(tmp/test.txt "${BUILD_TYPE_FROM_CLI}")

# set(CMAKE_GENERATOR "Ninja")
# message("generator is set to ${CMAKE_GENERATOR}")

# Architecture
message("Architecture:        ${CMAKE_SYSTEM_PROCESSOR}")
# 64 or 32 bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  message("Bits:                64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
  message("Bits:                32")
else ()
  message("Bits:                unknown")
endif()
# print platform
if(WIN32)
    message("Platform:            WIN32")
elseif(APPLE)
    message("Platform:            APPLE")
elseif(UNIX)
    set(LINUX TRUE)
    add_definitions(-DPH_LINUX)
    message("Platform:            LINUX")
endif()
# print compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message("Compiler :           Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message("Compiler :           GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  message("Compiler :       Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message("Compiler :       MSVC")
endif()
# Generator
message("Current generator:   ${CMAKE_GENERATOR}")
# Build Type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Build type:          Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wdeprecated-declarations") 
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")  
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Build type:          Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Perf")
    message("Build type:          Perf")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg") # NECESSARY TO GENERATE `gmon.out`
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
else()
    message("Build type:          Unknown")
endif()
# Debug flag
message("Debug flags:         ${CMAKE_CXX_FLAGS_DEBUG}")
message("Compile flags (C):   ${CMAKE_C_FLAGS}")
message("Compile flags (C++): ${CMAKE_CXX_FLAGS}")
message("Comp. f. (C++, env): ${CXXFLAGS}")
message("C Flags (Debug):     ${CMAKE_C_FLAGS_DEBUG}")
message("CXX Flags (Debug):   ${CMAKE_CXX_FLAGS_DEBUG}")
message("C Flags (Release):   ${CMAKE_C_FLAGS_RELEASE}")
message("CXX Flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")
# VARIABLES
message("CMAKE_INCLUDE_CURRENT_DIR: ${CMAKE_INCLUDE_CURRENT_DIR}") # add source and bin dirs automatically




# set up GLAD
add_library(glad extern/glad/src/glad.c)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
target_include_directories(glad PUBLIC extern/glad/include)
set(glad_header ${CMAKE_SOURCE_DIR}/extern/glad/include/glad/glad.h)



# Add all the source-files as a library
# add_subdirectory(source)
add_subdirectory(src)
# add_subdirectory(xp)

# Link the GLFW precompiled static library
# set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/libglfw3.a) # old path
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a)


# MAIN
add_executable(physimos src/main.cpp ${glad_header} )

# Link : needs to be after add_exectuable()!
target_link_libraries(physimos 
  source 
  ui 
  uic 
  uic_draw 
  uic_main_view 
  uic_right_panel 
  scene 
  draw 
  logger
  io 
  simulator 
  math 
  rend 
  lib 
  glad 
  ${GLFW_LIB}
)
# target_link_libraries(physimos source glad ${GLFW_LIB})


#  PHONT
# add_executable(phont_main 
#     xp/phont/main.cc 
#     ${glad_header} 
# )
# target_link_libraries(phont_main 
#     phont
#     glad 
#     ${GLFW_LIB}
# )


# GPT
# target_link_libraries(physimos X11 Xrandr Xi Xxf86vm Xinerama Xcursor GL dl pthread)
