cmake_minimum_required(VERSION 3.18)

# Name of project
project(music_formatter VERSION 0.0.1 LANGUAGES CXX)

set(APP_HUMAN_NAME "Music Formatter (C++)")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_CLI "Build CLI executable" ON)
option(BUILD_GUI "Build GUI executable" ON)

find_package(TagLib REQUIRED)
if(NOT TagLib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        TagLib
        GIT_REPOSITORY https://github.com/taglib/taglib.git
        GIT_TAG v2.1.1
    )
endif(NOT TagLib_FOUND)
find_package(spdlog REQUIRED)
if(NOT spdlog_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
    )
endif(NOT spdlog_FOUND)
find_package(Catch2 QUIET)
find_package(nlohmann_json 3 QUIET)

file(GLOB_RECURSE UI_SOURCES "src/ui/*.cpp")
file(GLOB_RECURSE MODEL_SOURCES "src/model/*.cpp")

include_directories("${CMAKE_SOURCE_DIR}/include")

if(Catch2_FOUND AND nlohmann_json_FOUND)
    # Make test program(s)
    add_executable(model_tests test/indexer_tests.cpp ${MODEL_SOURCES})

    target_link_libraries(model_tests 
    PRIVATE
        spdlog::spdlog
        TagLib::TagLib
        Catch2::Catch2WithMain
        nlohmann_json::nlohmann_json
    )

    set_target_properties(model_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/Testing"
    )

    include(CTest)
    include(Catch)

    catch_discover_tests(model_tests)
else(Catch2_FOUND AND nlohmann_json_FOUND)
    if(NOT Catch2_FOUND)
        message(WARNING "Catch2 not found")
    endif(NOT Catch2_FOUND)
    if(NOT nlohmann_json_FOUND)
        message(WARNING "Nlohmann JSON not found")
    endif(NOT nlohmann_json_FOUND)
    message(WARNING "Some test libraries not found, skipping tests")
endif(Catch2_FOUND AND nlohmann_json_FOUND)

if(BUILD_CLI)
    find_package(argparse REQUIRED)
    if(NOT argparse_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            argparse
            GIT_REPOSITORY https://github.com/p-ranav/argparse.git
            GIT_TAG v3.0
        )
    endif(NOT argparse_FOUND)

    message("Building cli program")
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
            argparse::argparse
    )
endif(BUILD_CLI)

if(BUILD_GUI)
    message("Building GUI program")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

    set(GTKMM_TARGET "")

    if(TARGET PkgConfig::GTKMM)
        set(GTKMM_TARGET PkgConfig::GTKMM)
    else()
        message(WARNING "gtkmm not found via pkg-config — falling back to FetchContent. This may build gtkmm from source and require many dependencies.")
        include(FetchContent)
        FetchContent_Declare(
            gtkmm
            GIT_REPOSITORY https://gitlab.gnome.org/GNOME/gtkmm.git
            GIT_TAG 4.10.0
        )
        FetchContent_MakeAvailable(gtkmm)

        if(TARGET gtkmm::gtkmm)
            set(GTKMM_TARGET gtkmm::gtkmm)
        elseif(TARGET gtkmm-4.0::gtkmm)
            set(GTKMM_TARGET gtkmm-4.0::gtkmm)
        else()
            message(FATAL_ERROR "Unable to determine gtkmm CMake target after FetchContent; inspect the fetched gtkmm targets")
        endif()
    endif()

    add_executable(music_formatter src/ui/control.cpp 
        ${UI_SOURCES}
        ${MODEL_SOURCES}
    )

    set_target_properties(music_formatter PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg/gui"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/gui"
        OUTPUT_NAME "music_formatter_${PROJECT_VERSION}"
    )

    target_link_libraries(music_formatter 
        PRIVATE
            ${GTKMM_TARGET}
            spdlog::spdlog
            TagLib::TagLib
    )
endif(BUILD_GUI)