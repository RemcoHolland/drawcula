cmake_minimum_required (VERSION 3.8)

project ("drawcula")

set(CMAKE_CXX_STANDARD 14)

# Bring the headers into the project
include_directories(include)

# Add sources, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

# Add source to this project's executable.
add_executable (drawcula ${SOURCES} )

################# Google Test ####################

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(drawculatest test/main.cpp test/perft_startposition.cpp test/perft_complexpositions test/matepositions.cpp test/stalematepositions.cpp src/perft.cpp src/magicmoves.cpp src/square.cpp src/stringutils.cpp src/moveinfo.cpp src/piece.cpp src/utils.cpp src/move.cpp src/board.cpp src/movelist.cpp src/fenreader.cpp src/feninfo.cpp src/movelist.cpp src/search.cpp src/evaluation.cpp src/uci.cpp)

target_link_libraries(
  drawculatest
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(drawculatest)

#################################################