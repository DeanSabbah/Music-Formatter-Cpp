# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C (and C++)
set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

set(MINGW_PREFIX "/usr/x86_64-w64-mingw32")
set(CMAKE_FIND_ROOT_PATH ${MINGW_PREFIX})

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Make pkg-config use a cross pkg-config wrapper (adjust path as needed)
# You can also set this on cmake command line with -DPKG_CONFIG_EXECUTABLE=...
set(PKG_CONFIG_EXECUTABLE "/usr/bin/x86_64-w64-mingw32-pkg-config" CACHE FILEPATH "pkg-config for the target")