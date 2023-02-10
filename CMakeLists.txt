cmake_minimum_required (VERSION 3.19)

# Set project info
project (
drawcula
VERSION 0.4.1
DESCRIPTION "Drawcula chess engine"
LANGUAGES CXX
)

# Set C++ version 20
set(CMAKE_CXX_STANDARD 20)

# Allow multithreading on Linux
if (UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

# Bring the headers into the project
include_directories(include)

# Add sources, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

# Add source to this project's executable.
add_executable (drawcula ${SOURCES} )

##################################################
################# Google Test ####################
##################################################

IF(CMAKE_BUILD_TYPE MATCHES Debug)  
	include(FetchContent)
	FetchContent_Declare(	  
	  googletest
	  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
	  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()

	add_executable(drawculatest test/main.cpp test/branching_factor.cpp test/perft_startposition.cpp test/perft_complexpositions.cpp test/play_game.cpp test/positions_mate.cpp test/positions_stalemate.cpp src/perft.cpp src/magicmoves.cpp src/square.cpp src/stringutils.cpp src/piece.cpp src/utils.cpp src/board.cpp src/movegen.cpp src/fenreader.cpp src/position.cpp src/search.cpp src/evaluation.cpp src/uci.cpp)

	target_link_libraries(
	  drawculatest
	  gtest_main
	)

	include(GoogleTest)
	gtest_discover_tests(drawculatest)

ENDIF()

#################################################