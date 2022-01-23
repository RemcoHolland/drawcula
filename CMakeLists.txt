# CMakeList.txt : CMake project for drawcula, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("drawcula")

# Bring the headers into the project
include_directories(include)

# Add sources, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

# Add source to this project's executable.
add_executable (drawcula ${SOURCES} )

# TODO: Add tests and install targets if needed.
