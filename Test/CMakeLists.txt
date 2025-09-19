include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(indexer_tests indexer_tests.cpp)

target_link_libraries(indexer_tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(indexer_tests PRIVATE "${CMAKE_SOURCE_DIR}/includes")

include(CTest)
include(Catch)
catch_discover_tests(indexer_tests)