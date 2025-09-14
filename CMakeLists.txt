cmake_minimum_required(VERSION 3.18)

# Name of project
set(PROJ_NAME "Music Formatter (C++)")
project(${PROJ_NAME})

find_package(TagLib REQUIRED)
file(GLOB APP_SOURCES "src/app/*.cpp")
file(GLOB MODEL_SOURCES "src/model/*.cpp")

add_executable(music_formatter src/main.cpp ${APP_SOURCES} ${MODEL_SOURCES})

set_target_properties(music_formatter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build"
    OUTPUT_NAME "main"
)


target_link_libraries(music_formatter PRIVATE TagLib::TagLib) 