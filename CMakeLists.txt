cmake_minimum_required(VERSION 3.18)

# Name of project
project(music_formatter VERSION 1.0.0 LANGUAGES CXX)

set(APP_HUMAN_NAME "Music Formatter (C++)")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
find_package(TagLib CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

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



message("Building GUI program")


add_executable(music_formatter src/main.cpp 
    ${UI_SOURCES}
    ${MODEL_SOURCES}
)

set_target_properties(music_formatter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dbg/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/release/"
    OUTPUT_NAME "music_formatter_${PROJECT_VERSION}"
)

target_link_libraries(music_formatter 
    PRIVATE
        PkgConfig::GTKMM
        spdlog::spdlog
        ZLIB::ZLIB
        TagLib::TagLib
        argparse::argparse
)

install(TARGETS music_formatter
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
    DESTINATION .
    OPTIONAL
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/
    DESTINATION .
    FILES_MATCHING
      PATTERN "*.so"
      PATTERN "*.so.*"
      PATTERN "*.dll"
      PATTERN "*.dylib"
    PATTERN "_CPack_Packages" EXCLUDE
    PATTERN "CMakeFiles" EXCLUDE
)

set(CPACK_PACKAGE_NAME "music_formatter")
set(CPACK_PACKAGE_VENDOR "Dean Sabbah")
set(CPACK_PACKAGE_CONTACT "deansabbah@cmail.carleton.ca")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Music Formatter (C++)")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MusicFormatter")

set(CPACK_GENERATOR "TGZ;ZIP;DEB") 

include(InstallRequiredSystemLibraries)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-4-1, zlib1g, libtagc0, libspdlog1")

include(CPack)