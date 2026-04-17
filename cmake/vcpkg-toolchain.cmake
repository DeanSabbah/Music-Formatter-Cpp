# cmake/vcpkg-toolchain.cmake
#
# Locates the vcpkg CMake integration script and includes it.
# Resolution order:
#   1. VCPKG_ROOT environment variable (set by vcpkg's own shell integration or
#      by CI/CD pipelines).
#   2. The vcpkg executable found on PATH – the directory containing the
#      executable *is* the vcpkg root, so no separate env-var is required.
#
# Usage: set CMAKE_TOOLCHAIN_FILE to this file (done via CMakePresets.json).

include_guard(GLOBAL)

if(DEFINED ENV{VCPKG_ROOT} AND NOT "$ENV{VCPKG_ROOT}" STREQUAL "")
    set(_VCPKG_ROOT "$ENV{VCPKG_ROOT}")
else()
    find_program(_VCPKG_EXECUTABLE NAMES vcpkg)
    if(NOT _VCPKG_EXECUTABLE)
        message(FATAL_ERROR
            "vcpkg not found. Either:\n"
            "  - set the VCPKG_ROOT environment variable, or\n"
            "  - add the vcpkg executable to PATH.")
    endif()
    get_filename_component(_VCPKG_EXECUTABLE_REAL "${_VCPKG_EXECUTABLE}" REALPATH)
    get_filename_component(_VCPKG_ROOT "${_VCPKG_EXECUTABLE_REAL}" DIRECTORY)
    unset(_VCPKG_EXECUTABLE CACHE)
endif()

set(_VCPKG_CMAKE "${_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if(NOT EXISTS "${_VCPKG_CMAKE}")
    message(FATAL_ERROR
        "vcpkg integration script not found at '${_VCPKG_CMAKE}'.\n"
        "Verify that VCPKG_ROOT (or the vcpkg binary on PATH) points to a "
        "valid vcpkg installation.")
endif()

include("${_VCPKG_CMAKE}")
