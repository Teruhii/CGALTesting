# CGALTesting
An exploration of the CGAL tools and functions

# Setup steps
- Install CMake (via Installer)
- Install Visual Studio (via Installer)
- Install vcpkg (via command line)
  - Either download the zip, or clone from github
- Install CGAL (via command line)
  - Done totally via command line in the location of vcpkg’s root folder
- Install qt5 for CGAL (via command line)
  - This is for visualising with the meshes
  - For lighter weight install, can just install the qt5[CGAL] module and not the entire qt5

# Project setup steps (From scratch)

- CMake only
  - Create folder of project
  - Create CMakeLists.txt file in root project folder
  - Create .cpp file for codes
  - Write codes in CMakeLists.txt to create a project and link CGAL and the required qt5 components to the project and executable
  - Make a ‘Build’ directory for building
  - Run ‘cmake-gui’ to configure the linking
    - Includes linking the location of the vcpkg.cmake file (x86 or x64 installed version of CGAL)
  - Run ‘cmake –build .’ to build the project and executable
  - Copy .ply file into the built executable’s root directory
    - This should be located deeper inside the ‘Build’ folder
  - Run the executable

- Visual Studio
  - Create project option in Visual Studio
  - Select to create a CMake project
  - Create .cpp file for codes
  - Write codes in CMakeLists.txt to create a project and link CGAL and the required qt5 components to the project and executable
  - Edit the ‘CMakePresets.json’ file to include “"CMAKE_TOOLCHAIN_FILE": "<PathTovcpkgRootFolder>\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"” under “cacheVariables”
    - Saving changes to the CMakeLists.txt causes Visual Studio to wipe the build directory and rebuild the executable and do the appropriate linking immediately
  - Copy .ply file into the built executable’s root directory
    - This should be located deeper inside the ‘Build’ folder
  - Run the executable via pressing the run ‘Current Document’ button on the top bar in the correct x64 or x86 configuration
