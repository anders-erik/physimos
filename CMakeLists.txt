# Top-level CMakeLists.txt

# Run commands
# $ mkdir build_cmake && cd build_cmake
# $ rm -r ./** && cmake .. && make && cd .. && ./build_cmake/physimos && cd ./build_cmake


cmake_minimum_required(VERSION 3.10)
project(physimos)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_GENERATOR "Ninja")
message("generator is set to ${CMAKE_GENERATOR}")

# set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)


# write_file(tmp/test.txt "${BUILD_TYPE_FROM_CLI}")
message([BUILD_TYPE_FROM_CLI] " = " "${BUILD_TYPE_FROM_CLI}")

if(${BUILD_TYPE_FROM_CLI}==DEBUG)

    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")             # 'regular' build
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")   # Gave me warnings that I cannot prioritize 

elseif(BUILD_TYPE_FROM_CLI==GPROF)

    set(CMAKE_BUILD_TYPE Gprof)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pg")       # gprof compiler flag

elseif(BUILD_TYPE_FROM_CLI==RELEASE_LINUX)

    set(CMAKE_BUILD_TYPE Release)

else() # Default build is debug

    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")             # 'regular' build

endif()



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
target_link_libraries(physimos source ui uic uic_draw uic_topbar uic_main_view uic_right_panel image scene draw logger res simulator math render lib glad ${GLFW_LIB})
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
