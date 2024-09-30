
add_library(logger
    logger.cc
)

target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)