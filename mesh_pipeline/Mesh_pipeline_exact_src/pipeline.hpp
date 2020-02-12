#ifndef PIPELINE
#define PIPELINE

#define BOOST_PARAMETER_MAX_ARITY  12  //to be at least 12. 

#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_root_of.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <fstream>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Polyhedral_complex_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; /*inexact*/
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

typedef CGAL::Surface_mesh<EK::Point_3> Mesh;
typedef boost::graph_traits<Mesh>::vertex_descriptor vertex_descriptor;        // may not be used 
typedef Mesh::Property_map<vertex_descriptor,EK::Point_3> Exact_point_map;     // may not be used 
typedef Mesh::Property_map<vertex_descriptor,bool> Exact_point_computed;       // may not be used 

#ifdef CGAL_CONCURRENT_MESH_3
typedef CGAL::Parallel_tag Concurrency_tag;
#else
typedef CGAL::Sequential_tag Concurrency_tag;
#endif

typedef CGAL::Mesh_polyhedron_3<K>::type Polyhedron;                 // used only in make_partitioned_tet_mesh.cpp
typedef CGAL::Polyhedral_complex_mesh_domain_3<K> Mesh_domain; //EK

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain,CGAL::Default,Concurrency_tag>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr,Mesh_domain::Corner_index,Mesh_domain::Curve_segment_index> C3t3;  //Curve_segment_index/*Curve_index*/
// Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

using namespace std;

struct Tissue{
       string name;
       int YoungsModulus;
       vector<Mesh> mesh_vec;
   };

int prog_opts(int ac, char* av[], string * in_dir, string * out_dir, float * edge_size);

bool substring_match(string e, string s);

int load_surface_meshes(string in_dir, vector<Tissue> &tissues);

int generate_subdomain_meshes(
    vector<Tissue> &tissues, 
    vector<Tissue> &subdomain_meshes, 
    vector<Mesh> &patch_vec, 
    vector<pair<int, int> >  &pair_vec, 
    string out_dir
);

int mkvec_cc(
    vector<Mesh> &patch_vec, 
    vector<pair<int, int> >  &pair_vec
);

int make_partitioned_tet_mesh(
    vector<Mesh> patch_vec, 
    const vector<pair<int, int> > pair_vec, 
    float edge_size_, 
    C3t3 &tet_mesh, 
    const string out_dir
);

#endif
