# cmake/vcpkg-toolchain.cmake
#
# Locates the vcpkg CMake integration script and includes it.
# Resolution order:
#   1. VCPKG_ROOT environment variable (set by vcpkg's own shell integration or
#      by CI/CD pipelines).
#   2. The vcpkg executable found on PATH – the directory containing the
#      executable *is* the vcpkg root, so no separate env-var is required.
#   3. Well-known default install locations (no PATH or env-var setup needed):
#        Linux / macOS : $HOME/vcpkg
#        Windows       : C:/vcpkg  |  C:/src/vcpkg  |  %USERPROFILE%/vcpkg
#
# Usage: set CMAKE_TOOLCHAIN_FILE to this file (done via CMakePresets.json).

include_guard(GLOBAL)

# ── Priority 1: VCPKG_ROOT env var ───────────────────────────────────────────
if(DEFINED ENV{VCPKG_ROOT} AND NOT "$ENV{VCPKG_ROOT}" STREQUAL "")
    set(_VCPKG_ROOT "$ENV{VCPKG_ROOT}")
else()
    # ── Priority 2: vcpkg binary on PATH ─────────────────────────────────────
    find_program(_VCPKG_EXECUTABLE NAMES vcpkg)
    if(_VCPKG_EXECUTABLE)
        get_filename_component(_VCPKG_EXECUTABLE_REAL "${_VCPKG_EXECUTABLE}" REALPATH)
        get_filename_component(_VCPKG_ROOT "${_VCPKG_EXECUTABLE_REAL}" DIRECTORY)
        unset(_VCPKG_EXECUTABLE CACHE)
        unset(_VCPKG_EXECUTABLE_REAL)
    else()
        unset(_VCPKG_EXECUTABLE CACHE)

        # ── Priority 3: conventional default install locations ────────────────
        set(_VCPKG_CANDIDATE_ROOTS "")
        if(DEFINED ENV{HOME})
            list(APPEND _VCPKG_CANDIDATE_ROOTS "$ENV{HOME}/vcpkg")
        endif()
        if(WIN32)
            list(APPEND _VCPKG_CANDIDATE_ROOTS "C:/vcpkg" "C:/src/vcpkg")
            if(DEFINED ENV{USERPROFILE})
                list(APPEND _VCPKG_CANDIDATE_ROOTS "$ENV{USERPROFILE}/vcpkg")
            endif()
        endif()

        foreach(_CANDIDATE IN LISTS _VCPKG_CANDIDATE_ROOTS)
            if(EXISTS "${_CANDIDATE}/scripts/buildsystems/vcpkg.cmake")
                set(_VCPKG_ROOT "${_CANDIDATE}")
                break()
            endif()
        endforeach()

        if(NOT DEFINED _VCPKG_ROOT)
            # Build a human-readable list of everything we tried.
            set(_VCPKG_TRIED
                "  - VCPKG_ROOT environment variable (not set)\n"
                "  - vcpkg executable on PATH (not found)\n"
                "  - Default locations checked:\n")
            foreach(_CANDIDATE IN LISTS _VCPKG_CANDIDATE_ROOTS)
                string(APPEND _VCPKG_TRIED "      ${_CANDIDATE}\n")
            endforeach()

            message(FATAL_ERROR
                "vcpkg not found. Tried:\n${_VCPKG_TRIED}"
                "To fix this, do one of the following:\n"
                "  - Set the VCPKG_ROOT environment variable to your vcpkg root, or\n"
                "  - Add the vcpkg executable to your PATH, or\n"
                "  - Install vcpkg to one of the default locations listed above.")
        endif()
    endif()
endif()

set(_VCPKG_CMAKE "${_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if(NOT EXISTS "${_VCPKG_CMAKE}")
    message(FATAL_ERROR
        "vcpkg integration script not found at '${_VCPKG_CMAKE}'.\n"
        "Verify that VCPKG_ROOT (or the vcpkg binary on PATH) points to a "
        "valid vcpkg installation.")
endif()

include("${_VCPKG_CMAKE}")
