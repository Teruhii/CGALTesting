// CGAL Basic
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>

// Drawing mesh
#include <CGAL/draw_surface_mesh.h>
#include <CGAL/draw_point_set_3.h>

// Preprocesssing
#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>

// CGAL Reconstruction
#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Scale_space_reconstruction_3/Jet_smoother.h>
#include <CGAL/Scale_space_reconstruction_3/Advancing_front_mesher.h>

// I/O
#include <fstream>

// Typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Sphere_3 Sphere_3;
typedef CGAL::Point_set_3<Point_3, Vector_3> Point_set;


int main(int argc, char* argv[])
{

	// Opening PLY file ------------------------------------------------------------------------------------
	const char* fname1 = "Level7DataCenterFinal.ply";
	std::ifstream input;
	input.open(fname1, std::ios_base::binary); // Opens the PLY file

	std::cout << "File name used: " << fname1 << "\n";

	if (input.fail()) {
		std::cout << "Failed failbit when opening ifstream input file\n";
	}

	std::cout << "Reading PLY from ifstream . . .\n";

	// Prepare objects for point and mesh manipulation
	Point_set pointInformation;
	Mesh initialSM; // Contains the information from the PLY file

	if (!CGAL::IO::read_PLY(input, pointInformation))
	{
		std::cerr << "Invalid input file." << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Success\n";
		//input >> pointInformation; // Put point information into the Point_set
		std::cout << "Number of points: " << pointInformation.number_of_points() << "\n";
	}


	// Down sample the point cloud -----------------------------------------------------------------------------------------

	bool skipDownSampling = false;


	if (!skipDownSampling) {
		// Compute average spacing using neighborhood of 6 points
		std::cout << "Computing average spacing between neighbours...\n";
		//double spacing = CGAL::compute_average_spacing<CGAL::Parallel_if_available_tag>(pointInformation, 6);
		//std::cout << "Computing complete. Average spacing is : " << spacing << "\n";

		std::cout << "Down sampling . . .\n";
		// Simplify using a grid of size 2 * average spacing
		typename Point_set::iterator gsim_it = CGAL::grid_simplify_point_set(pointInformation, 0.01);
		pointInformation.remove(gsim_it, pointInformation.end());
		std::cout << pointInformation.number_of_removed_points()
			<< " point(s) removed after simplification." << std::endl;
		pointInformation.collect_garbage();
		std::cout << "Down sampling complete\n";
	}


	// Outlier removal -----------------------------------------------------------------------------------------

	//CGAL::IO::read_PLY(input, pointInformation);
	bool skipOutlierRemoval = false;
	int numberOfNeighbours = 24;
	double pointRemovalPercentage = 5.0;

	if (!skipOutlierRemoval) {
		std::cout << "Removing outliers . . .\n";
		typename Point_set::iterator rout_it = CGAL::remove_outliers<CGAL::Parallel_if_available_tag>
			(pointInformation,
				numberOfNeighbours, // Number of neighbors considered for evaluation
				pointInformation.parameters().threshold_percent(pointRemovalPercentage)); // Percentage of points to remove
		pointInformation.remove(rout_it, pointInformation.end());
		std::cout << pointInformation.number_of_removed_points()
			<< " point(s) are outliers." << std::endl;
		// Applying point set processing algorithm to a CGAL::Point_set_3
		// object does not erase the points from memory but place them in
		// the garbage of the object: memory can be freeed by the user.
		pointInformation.collect_garbage();
		std::cout << "Outliers removed \n";
	}



	// Surface Reconstruction -----------------------------------------------------------------------------------------
	int POISSON = 0;
	int ADVANCINGFRONT = 1;
	int SCALESPACE = 3;

	int chosenType = 1;
	bool skipSurfaceReconstruction = true;

	typedef std::array<std::size_t, 3> Facet; // Triple of indices
	std::vector<Facet> facets;
	if (!skipSurfaceReconstruction) {

		// Poisson -----------------------------------------------------------------------------------------
		if (chosenType == POISSON) {
			std::cout << "Inside poisson\n";
			std::cout << "Estimating normals...\n";
			/*int noOfNeighbours = 24;
			CGAL::Surface_mesh<Point_3> output_mesh;
			CGAL::jet_estimate_normals<CGAL::Sequential_tag>
				(pointInformation, noOfNeighbours); // Use 24 neighbors
			  // Orientation of normals, returns iterator to first unoriented point
			typename Point_set::iterator unoriented_points_begin =
				CGAL::mst_orient_normals(pointInformation, 24); // Use 24 neighbors
			pointInformation.remove(unoriented_points_begin, pointInformation.end());
			std::cout << "Normals complete\n";
			std::cout << "Executing poisson...\n";
			CGAL::poisson_surface_reconstruction_delaunay
			(pointInformation.begin(), pointInformation.end(),
				pointInformation.point_map(), pointInformation.normal_map(),
				output_mesh, 0.01);

			std::cout << "Poisson complete\n";*/
		}
		else if (chosenType == ADVANCINGFRONT) { // Advancing Front -----------------------------------------------------------------------------------------

			std::cout << "Inside Advancing front\n";
			std::cout << "Executing Advancing front...\n";
			// The function is called using directly the points raw iterators
			CGAL::advancing_front_surface_reconstruction(pointInformation.points().begin(),
				pointInformation.points().end(),
				std::back_inserter(facets));
			std::cout << facets.size()
				<< " facet(s) generated by reconstruction." << std::endl;
			std::cout << "Advancing front complete\n";
		}
		else if (chosenType == SCALESPACE) { // Scale Space -------------------------------------------------
			std::cout << "Inside Scale space\n";
			std::cout << "Executing Scale space...\n";
			CGAL::Scale_space_surface_reconstruction_3<Kernel> reconstruct
			(pointInformation.points().begin(), pointInformation.points().end());
			// Smooth using 4 iterations of Jet Smoothing
			//reconstruct.increase_scale(4, CGAL::Scale_space_reconstruction_3::Jet_smoother<Kernel>());
			// Mesh with the Advancing Front mesher with a maximum facet length of 0.5
			reconstruct.reconstruct_surface(CGAL::Scale_space_reconstruction_3::Advancing_front_mesher<Kernel>(0.5));
			std::cout << "Scale space complete\n";
		}
	}




	// Draw the mesh ---------------------------------------------------------------------------------------------------

	std::cout << "Drawing ...\n";
	if (chosenType == POISSON) {
		CGAL::draw(initialSM);

	}
	else
	{
		CGAL::draw(pointInformation);

	}
	return EXIT_SUCCESS;
}