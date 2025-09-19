cmake_minimum_required(VERSION 3.18)

# Name of project
project(music_formatter VERSION 0.0.1 LANGUAGES CXX)

set(APP_HUMAN_NAME "Music Formatter (C++)")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TagLib REQUIRED)
find_package(spdlog REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Catch2)

pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

file(GLOB UI_SOURCES "src/ui/*.cpp")
file(GLOB MODEL_SOURCES "src/model/*.cpp")

include_directories("${CMAKE_SOURCE_DIR}/include")

if(Catch2_FOUND)
    # Make test program(s)
    add_executable(model_tests test/indexer_tests.cpp ${MODEL_SOURCES})

    target_link_libraries(model_tests 
    PRIVATE
        spdlog::spdlog
        TagLib::TagLib
        Catch2::Catch2WithMain
    )

    set_target_properties(model_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/Testing"
    )

    include(CTest)
    include(Catch)

    catch_discover_tests(model_tests)
endif(Catch2_FOUND)

add_executable(music_formatter_cli src/main.cpp ${MODEL_SOURCES})

set_target_properties(music_formatter_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg/cli"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/cli"
    OUTPUT_NAME "music_formatter_cli_${PROJECT_VERSION}"
)

target_link_libraries(music_formatter_cli 
    PRIVATE
        spdlog::spdlog
        TagLib::TagLib
)

add_executable(music_formatter src/ui/ui_main.cpp 
    ${UI_SOURCES}
    ${MODEL_SOURCES}
)

set_target_properties(music_formatter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg/gui"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/gui"
    OUTPUT_NAME "music_formatter_cli_${PROJECT_VERSION}"
)

target_link_libraries(music_formatter 
    PRIVATE
        PkgConfig::GTKMM
        spdlog::spdlog
        TagLib::TagLib
)