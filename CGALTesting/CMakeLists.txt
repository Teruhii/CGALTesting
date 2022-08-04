# CMakeList.txt : CMake project for plsworkthistime, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CGALTesting")

find_package(CGAL CONFIG REQUIRED)



# Add source to this project's executable.
add_executable (CGALTesting "CGALTesting.cpp" "CGALTesting.h")

target_link_libraries(CGALTesting CGAL::CGAL)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CGALTesting PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
