
add_library(simulator 
    Simulator.cpp
)

target_include_directories(simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
# For "Types.hpp"
target_include_directories(simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)

