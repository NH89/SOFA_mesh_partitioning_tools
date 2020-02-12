

//******************************************************************************
// File Description :
// Outputs to out.mesh a mesh of implicit domains.
//******************************************************************************



#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Implicit_to_labeling_function_wrapper.h>
#include <CGAL/Labeled_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>

// IO
#include <CGAL/IO/File_medit.h>

using namespace CGAL::parameters;

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK; //EK

typedef K::Point_3 Point;
typedef EK::Point_3 Point_exact;


typedef K::FT FT;
typedef EK::FT E_FT;

typedef FT (*Function)(const Point&);
typedef E_FT (*E_Function)(const Point_exact&);

typedef CGAL::Implicit_multi_domain_to_labeling_function_wrapper<Function>  Function_wrapper;
typedef CGAL::Implicit_multi_domain_to_labeling_function_wrapper<E_Function>  E_Function_wrapper;

typedef Function_wrapper::Function_vector Function_vector;
typedef E_Function_wrapper::Function_vector E_Function_vector;

typedef CGAL::Labeled_mesh_domain_3<K> Mesh_domain;
typedef CGAL::Labeled_mesh_domain_3<EK> E_Mesh_domain;

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_triangulation_3<E_Mesh_domain>::type E_Tr;

typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
typedef CGAL::Mesh_complex_3_in_triangulation_3<E_Tr> E_C3t3;

// Mesh Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
typedef CGAL::Mesh_criteria_3<E_Tr> E_Mesh_criteria;

typedef Mesh_criteria::Facet_criteria    Facet_criteria;
typedef E_Mesh_criteria::Facet_criteria    E_Facet_criteria;

typedef Mesh_criteria::Cell_criteria     Cell_criteria;
typedef E_Mesh_criteria::Cell_criteria     E_Cell_criteria;


double cube_function_1 (const Point& p)  // Point
{
  if( p.x() >= 0 && p.x() <= 2 &&
      p.y() >= 0 && p.y() <= 2 &&
      p.z() >= 0 && p.z() <= 2 )
    return -1.;
  return 1.;
}

E_FT /*double*/ e_cube_function_1 (const Point_exact& p)  // Point
{
  if( p.x() >= 0 && p.x() <= 2 &&
      p.y() >= 0 && p.y() <= 2 &&
      p.z() >= 0 && p.z() <= 2 )
    return -1.;
  return 1.;
}


double cube_function_2 (const Point& p)  // Point
{
  if( p.x() >= 1 && p.x() <= 3 &&
      p.y() >= 1 && p.y() <= 3 &&
      p.z() >= 1 && p.z() <= 3 )
    return -1.;
  return 1.;
}

E_FT /*double*/ e_cube_function_2 (const Point_exact& p)  // Point
{
  if( p.x() >= 1 && p.x() <= 3 &&
      p.y() >= 1 && p.y() <= 3 &&
      p.z() >= 1 && p.z() <= 3 )
    return -1.;
  return 1.;
}

int main()
{
  // Define functions
  Function f1 = cube_function_1;
  Function f2 = cube_function_2;
  E_Function e_f1 = e_cube_function_1;
  E_Function e_f2 = e_cube_function_2;

  Function_vector v;
  v.push_back(f1);
  v.push_back(f2);
  
  E_Function_vector e_v;
  e_v.push_back(e_f1);
  e_v.push_back(e_f2);
  
  std::vector<std::string> vps;
  vps.push_back("--");

  // Domain (Warning: Sphere_3 constructor uses square radius !)
  Mesh_domain domain(Function_wrapper(v, vps), K::Sphere_3(CGAL::ORIGIN, 5.*5.));
  
  E_Mesh_domain e_domain(E_Function_wrapper(e_v, vps), EK::Sphere_3(CGAL::ORIGIN, 5.*5.));
  

  // Set mesh criteria
  Mesh_criteria criteria(edge_size = 0.15,
      facet_angle = 30, facet_size = 0.2,
      cell_radius_edge_ratio = 2, cell_size = 0.4);
  E_Mesh_criteria e_criteria(edge_size = 0.15,
      facet_angle = 30, facet_size = 0.2,
      cell_radius_edge_ratio = 2, cell_size = 0.4);
  
  
  // Mesh generation
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_exude(), no_perturb());
  
  E_C3t3 e_c3t3 = CGAL::make_mesh_3<E_C3t3>(e_domain, e_criteria, no_exude(), no_perturb());

  // Perturbation (maximum cpu time: 10s, targeted dihedral angle: default)
  CGAL::perturb_mesh_3(c3t3, domain, time_limit = 10);
  
  CGAL::perturb_mesh_3(e_c3t3, e_domain, time_limit = 10);
  
  // Exudation
  CGAL::exude_mesh_3(c3t3,12);
  
  CGAL::exude_mesh_3(e_c3t3,12);
  
  // Output
  std::ofstream medit_file("out_cubes_intersection.mesh");
  std::ofstream e_medit_file("out_cubes_intersection_exact.mesh");
  
  CGAL::output_to_medit(medit_file, c3t3);
  CGAL::output_to_medit(e_medit_file, e_c3t3);

  
  return 0;
}
