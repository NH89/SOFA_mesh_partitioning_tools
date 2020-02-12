
//#include "pipeline.hpp"
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
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
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef CGAL::Surface_mesh<EK::Point_3> Mesh_exact;

using namespace std;

//#include <CGAL/MP_Float.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>

typedef K::Compare_dihedral_angle_3                    Compare_dihedral_angle_3;

typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;
typedef boost::graph_traits<Mesh>::face_descriptor     face_descriptor;

//typedef CGAL::Quotient<CGAL::MP_Float> mp_number;

template <typename G>
struct Constraint : public boost::put_get_helper<bool,Constraint<G> >
{
  typedef typename boost::graph_traits<G>::edge_descriptor edge_descriptor;
  typedef boost::readable_property_map_tag      category;
  typedef bool                                  value_type;
  typedef bool                                  reference;
  typedef edge_descriptor                       key_type;

  Constraint()
    :g_(NULL)
  {}

  Constraint(G& g, double bound) 
    : g_(&g), bound_(bound)
  {}

  bool operator[](edge_descriptor e) const
  {
    const G& g = *g_;
    return compare_(g.point(source(e, g)),
                    g.point(target(e, g)),
                    g.point(target(next(halfedge(e, g), g), g)),
                    g.point(target(next(opposite(halfedge(e, g), g), g), g)),
                   bound_) == CGAL::SMALLER;
  }
  
  const G* g_;
  Compare_dihedral_angle_3 compare_;
  double bound_;
};

struct halfedge2edge
{
  halfedge2edge(const Mesh& m, vector<edge_descriptor>& edges)
    : m_mesh(m), m_edges(edges)
  {}
  void operator()(const halfedge_descriptor& h) const
  {
    m_edges.push_back(edge(h, m_mesh));
  }
  const Mesh& m_mesh;
  vector<edge_descriptor>& m_edges;
};

using namespace CGAL ;

using namespace Polygon_mesh_processing;

template <typename PolygonMesh
        , typename NamedParameters>
size_t make_vector_of_connected_components(
    PolygonMesh& pmesh, 
    pair<int, int> pair_,
    vector<PolygonMesh> &mesh_vec, 
    vector<pair<int, int> >  &pair_vec,
    const NamedParameters& np
)
{ 
  typedef PolygonMesh PM;
  typedef typename boost::graph_traits<PM>::face_descriptor face_descriptor;
  using boost::choose_param;
  using boost::get_param;
                                                                            //FaceIndexMap
  typedef typename GetFaceIndexMap<PM,  NamedParameters>::type FaceIndexMap;
  FaceIndexMap fimap = choose_param(get_param(np, internal_np::face_index),
                                    get_property_map(boost::face_index, pmesh));
                                                                            //vector_property_map
  boost::vector_property_map<size_t, FaceIndexMap> face_cc(fimap);
  size_t num = connected_components(pmesh, face_cc, np);
  vector< pair<size_t, size_t> > component_size(num);

  for(size_t i=0; i < num; i++)
    component_size[i] = make_pair(i,0);

  BOOST_FOREACH(face_descriptor f, faces(pmesh))
    ++component_size[face_cc[f]].second;
                                                // we sort the range [0, num) by component size
  sort(component_size.begin(), component_size.end(), PMP::internal::MoreSecond());
  vector<size_t> cc_to_keep;
  
  for(size_t i=0; i<num; ++i) {
      PM tmp_mesh = pmesh;
      cc_to_keep.clear();
      cc_to_keep.push_back( component_size[i].first );
      keep_connected_components(tmp_mesh, cc_to_keep, face_cc, np);
      mesh_vec.push_back(tmp_mesh);
      pair_vec.push_back(pair_);
  }
  int indx=0;                                   // dbg verify written to mesh_vec
  BOOST_FOREACH(PM cc_mesh , mesh_vec){
      string filename =  "data/blobby_vec";
      filename +=  to_string(indx) + ".off";
      ofstream outfile(filename);
      outfile << cc_mesh;
      outfile.close();
      indx++;
  }
  return num;
}


int mkvec_cc(
        vector<Mesh_exact> &patch_vec, 
        vector<pair<int, int> >  &pair_vec,
        std::string out_dir
            )
{
    boost::filesystem::path p (out_dir.c_str()); 
    vector<Mesh_exact> new_patch_vec;
    vector<Mesh_exact> new_patch_vec2;
    vector<pair<int, int> >  new_pair_vec;
    const double bound = cos(0.75 * CGAL_PI);
    
    int idx =0;
    BOOST_FOREACH(Mesh_exact pmesh, patch_vec){
        make_vector_of_connected_components(pmesh, pair_vec.at(idx), new_patch_vec, new_pair_vec,
                PMP::parameters::edge_is_constrained_map(Constraint<Mesh_exact>(pmesh, bound)) );
        idx++;    // no match for epick  epeck
    }
    
    for(int i=0; i< patch_vec.size() ;i++){
        std::string filename = p.c_str();
        filename += "/patch_num_" + to_string(i) + ".off";
        std::ofstream output(filename);
        output << patch_vec.at(i);
        output.close();
    }

    patch_vec.swap(new_patch_vec); /*new_patch_vec2*/
    pair_vec.swap(new_pair_vec);
    return patch_vec.size();
}


// int iso_remesh(Mesh &nmesh, unsigned int nb_iter, double target_edge_length){
//     vector<edge_descriptor> border;
//     PMP::border_halfedges(
//                 faces(nmesh),
//                 nmesh,
//                 boost::make_function_output_iterator(halfedge2edge(nmesh, border))
//                 );
//     PMP::split_long_edges(border, target_edge_length, nmesh);
//         
//     PMP::isotropic_remeshing(
//                 faces(nmesh),
//                 target_edge_length,
//                 nmesh,                                    //protect border, here
//                 PMP::parameters::number_of_iterations(nb_iter).protect_constraints(true)
//                 );   
// }

