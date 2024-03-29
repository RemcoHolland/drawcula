﻿cmake_minimum_required (VERSION 3.19)

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
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Add source to this project's executable.
add_executable (drawcula ${SOURCES} )

##################################################
################# Google Test ####################
##################################################

IF(CMAKE_BUILD_TYPE MATCHES Debug)  
	set(GOOGLE_TEST_VERSION 1.13.0)
	
	include(FetchContent)
	FetchContent_Declare(	  
	  googletest
	  URL https://github.com/google/googletest/archive/refs/tags/v${GOOGLE_TEST_VERSION}.zip
	  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()
    
	# Create a text executable but exclude src/main.cpp
	file(GLOB_RECURSE TEST_SOURCES "test/*.cpp" "src/*.cpp")	
	list(FILTER TEST_SOURCES EXCLUDE REGEX src/main.cpp)
	add_executable(drawculatest ${TEST_SOURCES})

	target_link_libraries(
	  drawculatest
	  gtest_main
	)

	include(GoogleTest)
	gtest_discover_tests(drawculatest)

ENDIF()

#################################################