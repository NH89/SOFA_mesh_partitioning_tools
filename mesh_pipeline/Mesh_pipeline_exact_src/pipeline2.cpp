
#include "pipeline.hpp"

int main(int ac, char* av[]){
    string in_dir, out_dir;
    //Mesh_criteria mesh_criteria;
    float edge_size;
    std::vector<Tissue> tissues;
    std::vector<Tissue> subdomain_meshes;                     // nb these are all surface meshes
    vector<Mesh> patch_vec;
    vector<std::pair<int, int> >  pair_vec;
    C3t3 tet_mesh;
    
    //// Program options 
    prog_opts(ac, av, &in_dir, &out_dir, &edge_size);
    std::cout << "\nprog_opts() done\n" << std::endl;

    //// Imports
    load_surface_meshes(in_dir, tissues);
    std::cout << "\nload_surface_meshes() done" << std::endl;

    //// Polygon mesh processing ////
    // Generate subdomain meshes & patches
    generate_subdomain_meshes(tissues, subdomain_meshes, patch_vec, pair_vec, out_dir);  
    std::cout << "\ngenerate_subdomain_meshes() done" << std::endl;
   

    // Split disconnected patches into separate files. & update pair_vec
    //split_patches(patch_vec, pair_vec);
    mkvec_cc(patch_vec, pair_vec);
    std::cout << "\nmkvec_cc() done" << std::endl;


    // 7 make partioned tetrahedral mesh
    make_partitioned_tet_mesh(patch_vec, pair_vec, edge_size, tet_mesh, out_dir);
    std::cout << "\nmake_partitioned_tet_mesh() done" << std::endl;


    

    //make_partitioned_tet_mesh(vector<Mesh> patch_vec, vector<pair<int, int> > pair_vec, C3t3 tet_mesh, const string out_dir)
    // i) make complex domain
    //      see mesh_polyhedral_complex.cpp
    //      typedef CGAL::Polyhedral_complex_mesh_domain_3<k> Mesh_domain
    //      Mesh_domain domain(patches.begin(), patches.end(),
    //                 incident_subdomains, incident_subdomains+nb_patches);
    // ii) make mesh
    //      CGAL::make_mesh_3<C3t3>(Domain, criteria);
    
    // 8 refine & optimise tetrahedral mesh
    
    // 9 per-element Young's modulus by tissue type
    
    // 10 get surface of tetmesh
    
    // 11 save files for Sofa
    
    // 12 generate .scn xml file for the model
    
    /////////
    
    // 13 sensing 

    // 14 actuation
    
    // 15 control
    return 0;
}
    
 /* compile with 
  *  nb req c++11 eg for to_string(..) 
  * 
g++ -std=gnu++11 -c -g3 -fPIC prog_opts.cpp 
g++ -std=gnu++11 -c -g3 -fPIC boost_regex_match.cpp  
g++ -std=gnu++11 -c -g3 -fPIC load_surface_meshes.cpp
g++ -std=gnu++11 -c -g3 -fPIC generate_subdomain_meshes.cpp
g++ -std=gnu++11 -c -g3 -fPIC mkvec_cc2.cpp
g++ -std=gnu++11 -c -g3 -fPIC make_partitioned_tet_mesh.cpp

g++ -std=gnu++11 -g3 -fPIC -o pipeline2 pipeline2.cpp prog_opts.o load_surface_meshes.o generate_subdomain_meshes.o boost_regex_match.o mkvec_cc2.o make_partitioned_tet_mesh.o   -L/usr/local/lib -rdynamic -lboost_regex -lboost_program_options -lboost_filesystem -lboost_system -lyaml-cpp -lCGAL -lboost_thread -lgmp -lmpfr -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -Wl,-rpath,/usr/local/lib


  *  Run in ~/projects/Mesh_pipeline  with 
  
  ./pipeline2 -i ./input_files/ -o ./output_files
   
  
  
  view  partitioned tetrahedral mesh .mesh file, and save .msh file for Sofa :
  
  gmsh output_files2/out.mesh 

  
  
  */    
