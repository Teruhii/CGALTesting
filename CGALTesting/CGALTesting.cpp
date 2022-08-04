// plsworkthistime.cpp : Defines the entry point for the application.
//

#include "CGALTesting.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>
#include <fstream>
typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;


using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
