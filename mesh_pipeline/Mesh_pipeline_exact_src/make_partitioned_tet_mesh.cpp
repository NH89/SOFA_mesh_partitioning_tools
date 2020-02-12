#include "pipeline.hpp"
#include <CGAL/IO/output_to_vtu.h>
#include <CGAL/IO/io.h>

using namespace CGAL::parameters;

    // 7 make partioned tetrahedral mesh
    int make_partitioned_tet_mesh(vector<Mesh> patch_vec, const vector<pair<int, int> > pair_vec, float edge_size_, C3t3 &tet_mesh, const string out_dir){ 
/*    
 *  Mesh tet_mesh, Mesh surface, 
    i) make complex domain
         see mesh_polyhedral_complex.cpp
         typedef CGAL::Polyhedral_complex_mesh_domain_3<k> Mesh_domain
         Mesh_domain domain(patches.begin(), patches.end(),
                    incident_subdomains, incident_subdomains+nb_patches);*/ 
        cout << "\npatch_vec.size() = " << patch_vec.size();
        cout << "\tpair_vec.size() = " << pair_vec.size() << "\n";
        const std::size_t nb_patches = patch_vec.size();
        CGAL_assertion(pair_vec.size() == nb_patches);
        std::vector<Polyhedron> patches(nb_patches);                                        
    
        // write .off, convert Epeck_surfaceMesh -> Epick_polyhedron 
        for (std::size_t i = 0; i < nb_patches; ++i){
            std::string filename = out_dir;
            filename += "/patch" + to_string(i) + ".off";
            std::ofstream outfile(filename);
            outfile << patch_vec.at(i);
            outfile.close();
            
            cout << "\n Wrote filename :" << filename ;
            
            // read in files to new datatype /////////////////////////////////
            std::ifstream infile(filename);
            infile >> patches.at(i);
            infile.close();
            cout << "\ni=" << i;
            cout << "\tpatch_vec.at(i).is_valid()  = " << patch_vec.at(i).is_valid()  ;
            cout << "\tpatch_vec.at(i).is_empty()  = " << patch_vec.at(i).is_empty()  ;
//             cout << "\tpatch_vec.at(i).size_of_facets()  = " << patch_vec.at(i).size_of_facets()  ;
//             cout << "\tpatch_vec.at(i) num facets  = " << distance(patch_vec.at(i).facets_end() , patch_vec.at(i).facets_begin() )  ;
            cout << "\tpatch_vec.at(i).number_of_faces   " << patch_vec.at(i).number_of_faces();
            cout << "\n";
            cout << "\tpatches.at(i).is_valid()  = " << patches.at(i).is_valid()  ;
            cout << "\tpatches.at(i).is_empty()  = " << patches.at(i).is_empty()  ;
            cout << "\tpatches.at(i).size_of_facets()  = " << patches.at(i).size_of_facets()  ;
            cout << "\tpatches.at(i) num facets  = " << distance(patches.at(i).facets_end() , patches.at(i).facets_begin() )  ;
            cout << "\tpatch_vec.at(i).number_of_faces   " << patch_vec.at(i).number_of_faces();
            cout << "\n";
        }
/*
        convert vector to array 
        pair* pair_array = &pair_vec[0] 
        std::vector<double> double_vec;
        double* double_arry = &double_vec[0];*/ 
        const pair<int, int>* incident_subdomains = &pair_vec[0];
        
        for (int i=0; i<nb_patches; ++i){
            cout << "\npair_vec[" << i << "] = (" << pair_vec[i].first << "," << pair_vec[i].second << ")";
        }
        
        cout << "\n\nGenerating domain\n\n" << flush;                                           // Create domain
        Mesh_domain domain(patches.begin(), patches.end(), incident_subdomains, incident_subdomains+nb_patches );

//        cout << "\n\nDetecting features\n\n" << flush;                                          //includes detection of borders
        //domain.detect_borders();    //domain.detect_features();  // both prevent make_mesh_3(...) from finishing.
        

//         cout << "\n\nSetting Mesh_criteria\n\n" << flush;                                       // Mesh criteria
//        Mesh_criteria criteria(edge_size = edge_size_/*0.03*/,
//                          facet_angle = 25, facet_size = edge_size_/*0.03*/, facet_distance = edge_size_/5  /*0.005*/,
//                          cell_radius_edge_ratio = 3, cell_size = edge_size_*10  /*0.4*/);
        
        Mesh_criteria criteria(edge_size = 4,
                         facet_angle = 25, facet_size = 4, facet_distance = 0.4,
                         cell_radius_edge_ratio = 3, cell_size = 4 );
        
        //edge_size = 8, facet_angle = 25, facet_size = 8, facet_distance = 0.2, cell_radius_edge_ratio = 3, cell_size = 10 // mesh_polyhedral_complex.cpp edge sizes 6.0 to 11
        
        // mesh_polyhedral_domain_with_lipschitz_sizing.cpp    edge sizes ~0.02
        //edge_size = min_size, facet_angle = 25, facet_size = min_size, facet_distance = 0.005, cell_radius_edge_ratio = 3, cell_size = lip_sizing 
        //FT min_size = 0.02;
        //lip_sizing.add_parameters_for_subdomain(1,       //subdomain id
                                          //0.3,     //k
                                          //min_size,//min_size
                                          //0.5);    //max_size

        cout << "\n\nGenerating tet_mesh\n\n" << flush;                                         // Mesh generation
        tet_mesh = CGAL::make_mesh_3<C3t3>(domain, /*mesh_*/criteria);
        
        cout << "\n\nSaving tet_mesh\n\n" << flush;                                             // Output
        string ofilename = out_dir;
        ofilename += "/out.mesh";
        std::ofstream medit_file(ofilename);
        tet_mesh.output_to_medit(medit_file);
        
        string ofilename_vtk = out_dir;
        ofilename_vtk += "/out.vtu";
        std::ofstream vtk_file(ofilename_vtk);
        //tet_mesh.output_to_vtu(vtk_file);
        CGAL::output_to_vtu(vtk_file, tet_mesh,  CGAL::IO::ASCII);
        
        
        
        
        cout << "\n\nFinished\n\n" << flush;
    // ii) make mesh
    //      CGAL::make_mesh_3<C3t3>(Domain, criteria);
    // 8 refine & optimise tetrahedral mesh
    // 9 per-element Young's modulus by tissue type
    // 10 get surface of tetmesh
    }
