# Top-level CMakeLists.txt

# Run commands
# $ mkdir build_cmake && cd build_cmake
# $ rm -r ./** && cmake .. && make && cd .. && ./build_cmake/physimos && cd ./build_cmake


set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra") # Gave me warnings that I cannot prioritize right now
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall ")


cmake_minimum_required(VERSION 3.10)
project(physimos)

set(CMAKE_CXX_STANDARD 17)

# set up GLAD
add_library(glad extern/glad/src/glad.c)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
target_include_directories(glad PUBLIC extern/glad/include)
set(glad_header ${CMAKE_SOURCE_DIR}/extern/glad/include/glad/glad.h)



# Add all the source-files as a library
# add_subdirectory(source)
add_subdirectory(src)


# Link the GLFW precompiled static library
# set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/libglfw3.a) # old path
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a)


# Define the executable 
add_executable(physimos src/main.cpp ${glad_header} )


# Link : needs to be after add_exectuable()!
target_link_libraries(physimos source logger res simulator math glad ${GLFW_LIB})
# target_link_libraries(physimos source glad ${GLFW_LIB})

# GPT
# target_link_libraries(physimos X11 Xrandr Xi Xxf86vm Xinerama Xcursor GL dl pthread)
