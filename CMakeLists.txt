# CMakeList.txt : CMake project for plsworkthistime, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CGALTesting")

# For CGAL only
find_package(CGAL CONFIG REQUIRED)

# For Qt5 for drawing
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)
create_single_source_cgal_program("sm_derivation.cpp")
create_single_source_cgal_program("sm_join.cpp")
create_single_source_cgal_program("sm_aabbtree.cpp")
create_single_source_cgal_program("sm_bgl.cpp")
create_single_source_cgal_program("sm_circulators.cpp")
create_single_source_cgal_program("sm_do_intersect.cpp")
create_single_source_cgal_program("sm_iterators.cpp")
create_single_source_cgal_program("sm_kruskal.cpp")
create_single_source_cgal_program("sm_memory.cpp")
create_single_source_cgal_program("sm_properties.cpp")
create_single_source_cgal_program("sm_draw_small_faces.cpp")
create_single_source_cgal_program("check_orientation.cpp")

# Add source to this project's executable.
add_executable (CGALTesting "CGALTesting.cpp" "CGALTesting.h")

# Link CGAL to the project
target_link_libraries(CGALTesting CGAL::CGAL CGAL::CGAL_Basic_viewer)

# Link qt5 to the project
if(CGAL_Qt5_FOUND)
  #link it with the required CGAL libraries
 #target_link_libraries(plsworkthistime PUBLIC CGAL::CGAL_Basic_viewer)
 #target_link_libraries(sm_draw_small_faces PUBLIC CGAL::CGAL_Basic_Viewer)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CGALTesting PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
