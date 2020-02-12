#include "pipeline.hpp"
#include <boost/filesystem.hpp>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/transform.h>

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <boost/function_output_iterator.hpp>

namespace PMP = CGAL::Polygon_mesh_processing;
namespace params = PMP::parameters;

typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;

using namespace boost::filesystem;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace params = PMP::parameters;

struct halfedge2edge   {
        halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges)
        : m_mesh(m), m_edges(edges)
        {}
        void operator()(const halfedge_descriptor& h) const
            {  m_edges.push_back(edge(h, m_mesh)); }
        const Mesh& m_mesh;
        std::vector<edge_descriptor>& m_edges;
    };

int generate_subdomain_meshes(
    std::vector<Tissue> &tissues, 
    std::vector<Tissue> &subdomain_meshes, 
    vector<Mesh> &patch_vec, 
    vector<std::pair<int, int> >  &pair_vec, 
    std::string out_dir
){
    cout << "\nout_dir = " << out_dir << "\n";
    path p (out_dir.c_str());                                                                                               // Move directory checking to prog_opts.cpp //#########
    try {
        if (!is_directory(p)){cout << p << "is not a directory\n";}
        else {  cout << p << " is a directory \n";}
    }catch (const filesystem_error& ex)  {cout << ex.what() << '\n';}
/*    
//   cout << "tissues.begin()->mesh_vec.begin()->is_empty()  = " 
//   << tissues.begin()->mesh_vec.begin()->is_empty() << "\n ";*/
    
    // store new meshes in std::vector<Tissue> subdomain_meshes 
    // nb "if any" applies to each step
    // For each tissue, store 
    //i)Union - senior tissues 
    //ii)remaining non-contact surface 
    //iii)each contact patch - named by tissues. 1 for each tissue type, make union, nb preserve features
    
    // build tissue unions
    for(std::vector<Tissue>::iterator tissue_it = tissues.begin(); tissue_it != tissues.end(); ++tissue_it) {
        if (  !(tissue_it->mesh_vec.size()) ) continue;
        Mesh union_msh = *tissue_it->mesh_vec.begin();
        
        for(std::vector<Mesh>::iterator mesh_it = tissue_it->mesh_vec.begin() +1; mesh_it != tissue_it->mesh_vec.end(); ++mesh_it) {                // i) for each tissue 
            PMP::corefine_and_compute_union( union_msh, *mesh_it,  union_msh );
        }//mesh_it
        Tissue new_tissue;
        new_tissue.name = tissue_it->name;
        new_tissue.YoungsModulus = tissue_it->YoungsModulus;
        new_tissue.mesh_vec.push_back(union_msh);
        new_tissue.mesh_vec.push_back(union_msh);
        subdomain_meshes.push_back(new_tissue);
    }//tissue_it  
    
/*
 *    // Save tissue unions for inspection
    for(std::vector<Tissue>::iterator tissue_it_a = subdomain_meshes.begin(); tissue_it_a != subdomain_meshes.end(); ++tissue_it_a) {
        if (!tissue_it_a->mesh_vec.at(1).is_empty() )   {     
            int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
            std::string filename = p.c_str();
            filename += "/tissue_union_" + to_string(iter_num_a) + tissue_it_a->name + ".off";
            cout << filename << endl;
            std::ofstream output(filename);
            output << tissue_it_a->mesh_vec.at(1);
            output.close();
        }
    }*///tissue_it_a
    
    cout << "subdomain_meshes.begin()->mesh_vec.begin()->is_empty()  = " << subdomain_meshes.begin()->mesh_vec.begin()->is_empty() << endl;
    cout << flush;
    // for each tissue ## generate patches ##
    for(std::vector<Tissue>::iterator tissue_it_a = subdomain_meshes.begin(); tissue_it_a != subdomain_meshes.end(); ++tissue_it_a) {               
        cout << "\n\ntissue_it_a = " << tissue_it_a - subdomain_meshes.begin()<< std::endl;
        
        for(std::vector<Tissue>::iterator tissue_it_b = subdomain_meshes.begin(); tissue_it_b != tissue_it_a; ++tissue_it_b) {                      // for each senior tissue ## generate interface patches ##
            cout << "\ntissue_it_b = " << tissue_it_b - subdomain_meshes.begin() << "\t" << std::flush;
            Tissue target = *tissue_it_a;
            
            for(std::vector<Tissue>::iterator tissue_it_c = subdomain_meshes.begin(); tissue_it_c != tissue_it_a; ++tissue_it_c) {                  // form union of all other senior tissues - may be the wrong approach.
            // instead, subtract each other tissue from the target.
                cout << "\ntissue_it_c = " << tissue_it_c - subdomain_meshes.begin()<< std::flush;
                if (tissue_it_c == tissue_it_b){
                    cout << "\tcontinue\n";
                    continue;
                }
                PMP::corefine_and_compute_difference(
                    target.mesh_vec.at(1),                           // target
                    tissue_it_c->mesh_vec.at(1),                     // clipper
                    target.mesh_vec.at(1)                             
                );
            }//tissue_it_c
            // 'target' now _should_ contain 'tissue_it_a', cropped by every tissue senior to 'a' except 'b'.  
            cout << "target.mesh_vec.at(1).is_empty() = " << target.mesh_vec.at(1).is_empty() << endl;
/*            
            if (!target.mesh_vec.at(1).is_empty() )   {     
                int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
                int iter_num_b = tissue_it_b - subdomain_meshes.begin() +1;
                std::string filename = p.c_str();
                filename += "/target_" + to_string(iter_num_a) + tissue_it_a->name + to_string(iter_num_b) + tissue_it_b->name + ".off";
                cout << filename << endl;
                std::ofstream output(filename);
                output << target.mesh_vec.at(1);
                output.close();
            }*/  
            Mesh target_mesh = tissue_it_b->mesh_vec.at(1);//target.mesh_vec.at(1);
            Mesh clipper_mesh = target.mesh_vec.at(1);
/*            
             if (!target_mesh.is_empty() )   {         //   target.mesh_vec.at(1)                                                                                                // name and save the patch
                int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
                int iter_num_b = tissue_it_b - subdomain_meshes.begin() +1;
                const std::pair<int, int> pair(iter_num_b, iter_num_a);
                cout << "\n target_mesh : " << iter_num_b << "_" << iter_num_a;

                std::string filename_t = p.c_str();
                filename_t += "/" + to_string(patch_vec.size()) + "target_mesh_" + to_string(iter_num_b) + tissue_it_b->name + "_" + to_string(iter_num_a) + tissue_it_a->name + ".off";
                std::ofstream output(filename_t);
                output << target_mesh;
                output.close();
             }*/  
/*
             if (!clipper_mesh.is_empty() )   {      // target.mesh_vec.at(1)                                                                                                    // name and save the patch
                int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
                int iter_num_b = tissue_it_b - subdomain_meshes.begin() +1;
                const std::pair<int, int> pair(iter_num_b, iter_num_a);
                cout << "\n clipper_mesh : " << iter_num_b << "_" << iter_num_a << endl;
                
                std::string filename_c = p.c_str();
                filename_c += "/" + to_string(patch_vec.size()) + "clipper_mesh_" + to_string(iter_num_b) + tissue_it_b->name + "_" + to_string(iter_num_a) + tissue_it_a->name + ".off";
                std::ofstream output(filename_c);
                output << clipper_mesh;
                output.close();
             }*/  
             
            PMP::corefine_and_compute_intersection(                                                                                            
                target_mesh,//target.mesh_vec.at(1),                           // target
                clipper_mesh,//tissue_it_b->mesh_vec.at(1),                     // clipper
                target_mesh,//target.mesh_vec.at(1),
                                                                                                                               
                params::clip_volume(false).face_index_map( get(CGAL::dynamic_face_property_t<std::size_t>(), target_mesh) ).use_bool_op_to_clip_surface(true),
                                                        
                params::face_index_map( get(CGAL::dynamic_face_property_t<std::size_t>(), clipper_mesh)  )
            );
            // 'target' now _should_ contain only the contact patch between 'a' and 'b'

            if (!target_mesh/*target.mesh_vec.at(1)*/.is_empty() )   {                                                                                                           // name and save the patch
                int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
                int iter_num_b = tissue_it_b - subdomain_meshes.begin() +1;
                const std::pair<int, int> pair(iter_num_b, iter_num_a);
                cout << "\n target patch : " << iter_num_b << "_" << iter_num_a;
/* */           patch_vec.push_back(target_mesh);
                pair_vec.push_back(pair);
                
                std::string filename = p.c_str();
                filename += "/" + to_string(patch_vec.size()) + "patch_" + to_string(iter_num_b) + tissue_it_b->name + "_" + to_string(iter_num_a) + tissue_it_a->name + ".off";
                std::ofstream output(filename);
                output << target_mesh;
                output.close();
             }
        }//tissue_it_b  // for each senior tissue ## generate interface patches ##

                                                                                                                                                    // ## generate freespace patches ##
        Mesh target = tissue_it_a->mesh_vec.at(1);
        for(std::vector<Tissue>::iterator tissue_it_d = subdomain_meshes.begin(); tissue_it_d != subdomain_meshes.end(); ++tissue_it_d) {           // form union of all other tissues
            if (tissue_it_d == tissue_it_a)continue;
            cout << "\ntissue_it_d = " << tissue_it_d - subdomain_meshes.begin() << std::endl;
            
            Mesh Clipper = tissue_it_d->mesh_vec.at(1);
            CGAL::Polygon_mesh_processing::reverse_face_orientations(Clipper);   // invert the clipper, leave what is outside all other subdomains.
            PMP::corefine_and_compute_intersection(
                target,
                Clipper, 
                target,
                            
                params::clip_volume(false).face_index_map( get(                                                       CGAL::dynamic_face_property_t<std::size_t>(), target ) ).use_bool_op_to_clip_surface(true),
                                                   
                params::face_index_map( get(CGAL::dynamic_face_property_t<std::size_t>(), Clipper))
                );
        }//tissue_it_d  // ## generate freespace patches ##
        // target should contain only the freespace patch of 'tissue_it_a'
        
        if (!target.is_empty() )   {
            int iter_num_a = tissue_it_a - subdomain_meshes.begin() +1;
            int iter_num_b = 0;                                                                                                                 // 0, i.e. free surface
            const std::pair<int, int> pair(iter_num_a, iter_num_b);
            cout << "\n target patch : " << iter_num_a << "_" << iter_num_b;
/* */       patch_vec.push_back(target);
            pair_vec.push_back(pair);
                
            std::string filename = p.c_str();
            filename += "/" + to_string(patch_vec.size()) + "patch_freespace_" + to_string(iter_num_a) + tissue_it_a->name + "_" + to_string(iter_num_b) + ".off";
            std::ofstream output(filename);
            output << target;
            output.close();
        }//if (!target.is_empty() )  
    }//tissue_it_a
    
/*    
    // write original patches to file for comparison
    const std::size_t nb_patches = patch_vec.size();
    for (std::size_t i = 0; i < nb_patches; ++i){
      std::string filename = out_dir;
      filename += "/original_patch" + to_string(i) + ".off";
      std::ofstream outfile(filename);
      outfile << patch_vec.at(i);
      outfile.close();
      cout << "\n Wrote filename :" << filename ;
    }
      
    ///////////////////////*/
    
    
    return patch_vec.size();
}
