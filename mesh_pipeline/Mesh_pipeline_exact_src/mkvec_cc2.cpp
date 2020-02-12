#include "pipeline.hpp"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>

#include <boost/property_map/property_map.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <fstream>
#include <map>

typedef CGAL::Exact_predicates_exact_constructions_kernel    Kernel;
typedef Kernel::Point_3                                      Point;

typedef CGAL::Surface_mesh<Point>                            Mesh;
typedef boost::graph_traits<Mesh>::face_descriptor           face_descriptor;
typedef boost::graph_traits<Mesh>::faces_size_type           faces_size_type;

typedef Mesh::Property_map<face_descriptor, faces_size_type> FCCmap;
typedef CGAL::Face_filtered_graph<Mesh> Filtered_graph;

typedef boost::graph_traits<Mesh>::halfedge_descriptor       halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor           edge_descriptor;


namespace PMP = CGAL::Polygon_mesh_processing;

using namespace std;

struct halfedge2edge
{
    halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges)
    : m_mesh(m), m_edges(edges)
    {}
    void operator()(const halfedge_descriptor& h) const
    {
        m_edges.push_back(edge(h, m_mesh));
    }
    const Mesh& m_mesh;
    std::vector<edge_descriptor>& m_edges;
};

double target_edge_length = 6.0;//4.0;//0.04;               // arbitrary remeshing parameters, move to input
unsigned int nb_iter = 2;//3;


size_t make_vector_of_connected_components(
    Mesh pmesh, 
    pair<int, int> pair_,
    vector<Mesh> &mesh_vec, 
    vector<pair<int, int> >  &pair_vec
){
    FCCmap fccmap = pmesh.add_property_map<face_descriptor, faces_size_type>("f:CC").first;
    faces_size_type num = PMP::connected_components(pmesh,fccmap);
    vector< pair<size_t, size_t> > component_size(num);

    for(size_t i=0; i < num; i++) component_size[i] = make_pair(i,0);
    
    BOOST_FOREACH(face_descriptor f, faces(pmesh))  ++component_size[fccmap[f]].second;
    
    sort(component_size.begin(), component_size.end(), PMP::internal::MoreSecond());
    vector<size_t> cc_to_keep;
    
    for(size_t i=0; i<num; ++i) {
      Mesh tmp_mesh = pmesh;
      cc_to_keep.clear();
      cc_to_keep.push_back( component_size[i].first );
      PMP::keep_connected_components(tmp_mesh, cc_to_keep, fccmap);
      // use defaultfor: const NamedParameters &  	np   above.
      if(tmp_mesh.number_of_faces () > 0 ){
          mesh_vec.push_back(tmp_mesh);
          pair_vec.push_back(pair_);
      }
    }
}

int mkvec_cc(
        vector<Mesh> &patch_vec, 
        vector<pair<int, int> >  &pair_vec
            )
{
    vector<Mesh> new_patch_vec;
    vector<pair<int, int> >  new_pair_vec;
    const double bound = cos(0.75 * CGAL_PI);
    
    int idx =0;
    BOOST_FOREACH(Mesh pmesh, patch_vec){
        cout << "\n idx = " << idx ;
        
        make_vector_of_connected_components( pmesh, pair_vec.at(idx), new_patch_vec, new_pair_vec );
        
        cout << ", new_pair_vec.size() = " << new_pair_vec.size();
        idx++;
    }
    
    for (int i=0; i<patch_vec.size(); ++i){
            cout << "\npair_vec[" << i << "] = (" << pair_vec[i].first << "," << pair_vec[i].second << ")";
    }
    
    for (int i=0; i<new_patch_vec.size(); ++i){
            cout << "\nnew_pair_vec[" << i << "] = (" << new_pair_vec[i].first << "," << new_pair_vec[i].second << ")";
    }
    
    patch_vec.swap(new_patch_vec);
    pair_vec.swap(new_pair_vec);
    
    // isotropic remeshing of each patch, to eliminate degenerate or ultra thin triangles. 
    // does _not_ fix degenerate triangles in tetrahedral meshing of certain polyhedra combinations
//     int i =0;
//     BOOST_FOREACH(Mesh pmesh, patch_vec){
//         cout << "\n remeshing patch" << endl;
//         std::vector<edge_descriptor> border;
//         PMP::border_halfedges(faces(pmesh),
//                           pmesh,
//                           boost::make_function_output_iterator(halfedge2edge(pmesh, border))
//                          );
//         PMP::split_long_edges(border, target_edge_length, pmesh);
//         PMP::isotropic_remeshing(faces(pmesh),
//                              target_edge_length,
//                              pmesh,
//                              PMP::parameters::number_of_iterations(nb_iter)
//                              .protect_constraints(true)//i.e. protect border, here  
//                              //can take a very long time if remesh triangles are too small
//                             );
//         patch_vec.at(i) = pmesh;
// //         std::string filename = "output_files2";
// //             filename += "/remeshed_patch" + to_string(i) + ".off";
// //             std::ofstream outfile(filename);
// //             outfile << pmesh;//patch_vec.at(i);
// //             outfile.close();
//         i++;    
//         
//         cout << "... finished remeshing patch" << endl;
//     }
//     cout << "\n finished all remeshing" << endl;
    return patch_vec.size();
}
