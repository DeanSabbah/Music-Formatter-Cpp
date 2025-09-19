cmake_minimum_required(VERSION 3.18)

# Name of project
project(music_formatter LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TagLib REQUIRED)
find_package(spdlog REQUIRED)
find_package(Catch2 3)

file(GLOB APP_SOURCES "src/app/*.cpp")
file(GLOB MODEL_SOURCES "src/model/*.cpp")

if(Catch2_FOUND)
    message("Building tests")
    add_executable(tests test/indexer_tests.cpp ${MODEL_SOURCES})

    target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/includes")

    target_link_libraries(tests
        PRIVATE
            spdlog::spdlog
            TagLib::TagLib
            Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif(Catch2)

add_executable(music_formatter src/main.cpp ${APP_SOURCES} ${MODEL_SOURCES})

set_target_properties(music_formatter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build"
    OUTPUT_NAME "main"
)

target_link_libraries(music_formatter
    PRIVATE 
       spdlog::spdlog
       TagLib::TagLib
)

target_include_directories(music_formatter PRIVATE "${CMAKE_SOURCE_DIR}/includes")