
#include "pipeline.hpp"
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>

typedef EK::Compare_dihedral_angle_3                    Compare_dihedral_angle_3;

typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;
typedef boost::graph_traits<Mesh>::face_descriptor     face_descriptor;

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

  Constraint(G& g, /*double*/ EK::FT bound) 
    : g_(&g), bound_(bound)
  {}

  bool operator[](edge_descriptor e) const
  {
    const G& g = *g_;
    return compare_(g.point(source(e, g)),
                    g.point(target(e, g)),
                    g.point(target(next(halfedge(e, g), g), g)),
                    g.point(target(next(opposite(halfedge(e, g), g), g), g) ),
                   bound_) == CGAL::SMALLER;
  }

  const G* g_;
  Compare_dihedral_angle_3 compare_;
  /*double*/ EK::FT bound_;
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
    cout << "\n pair = (" << pair_.first << "," << pair_.second << ") ";
    cout << ", mesh_vec.size() = " << mesh_vec.size();
    cout << ", pair_vec.size() = " << pair_vec.size();
    
  typedef PolygonMesh PM;
  typedef typename boost::graph_traits<PM>::face_descriptor face_descriptor;
  using boost::choose_param;
  using boost::get_param;
                                                                            //FaceIndexMap
  typedef typename GetFaceIndexMap<PM,  NamedParameters>::type FaceIndexMap;
  
  FaceIndexMap fimap = choose_param(get_param(np, internal_np::face_index),
                                    get_property_map(boost::face_index, pmesh)
                                   );
                                                                            //vector_property_map
  boost::vector_property_map<size_t, FaceIndexMap> face_cc(fimap);
  
  size_t num = connected_components(pmesh, face_cc, np);                    // num  
  
  vector< pair<size_t, size_t> > component_size(num);

  for(size_t i=0; i < num; i++)
    component_size[i] = make_pair(i,0);

  BOOST_FOREACH(face_descriptor f, faces(pmesh))
    ++component_size[face_cc[f]].second;
                                                // sort the range [0, num) by component size
  sort(component_size.begin(), component_size.end(), PMP::internal::MoreSecond());
  
  vector<size_t> cc_to_keep;
  
  
  for(size_t i=0; i<num; ++i) {
      PM tmp_mesh = pmesh;
      cc_to_keep.clear();
      
      cc_to_keep.push_back( component_size[i].first );
      
      keep_connected_components(tmp_mesh, cc_to_keep, face_cc, np);
      
      if(tmp_mesh.number_of_faces () > 0 ){
          mesh_vec.push_back(tmp_mesh);
          pair_vec.push_back(pair_);
      }
  }
/*  
  int indx=0;                                   // dbg verify written to mesh_vec
  BOOST_FOREACH(PM cc_mesh , mesh_vec){
      string filename =  "data/blobby_vec";
      filename +=  to_string(indx) + ".off";
      ofstream outfile(filename);
      outfile << cc_mesh;
      outfile.close();
      indx++;
  }*/  
  return num;
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
        make_vector_of_connected_components(pmesh, pair_vec.at(idx), new_patch_vec, new_pair_vec,
                PMP::parameters::edge_is_constrained_map(Constraint<Mesh>(pmesh, bound)) );
        
        make_vector_of_connected_components(
            pmesh, pair_vec.at(idx), new_patch_vec, new_pair_vec
        );//new version without dihedral angle bound
        
        cout << ", new_pair_vec.size() = " << new_pair_vec.size();
        idx++;
    }
    
    for (int i=0; i<patch_vec.size(); ++i){
            cout << "\npair_vec[" << i << "] = (" << pair_vec[i].first << "," << pair_vec[i].second << ")";
    }
    
    for (int i=0; i<new_patch_vec.size(); ++i){
            cout << "\nnew_pair_vec[" << i << "] = (" << new_pair_vec[i].first << "," << new_pair_vec[i].second << ")";
    }
    
    patch_vec.swap(new_patch_vec); /*new_patch_vec2*/
    pair_vec.swap(new_pair_vec);
    return patch_vec.size();
}
