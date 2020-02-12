#include "pipeline.hpp"
#include <cstdlib>
#include <boost/regex.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include <CGAL/Polygon_mesh_processing/remesh.h>

// To avoid verbose function and named parameters call
using namespace boost::filesystem;
using namespace CGAL::parameters;
using namespace std;

const double target_edge_length = 4.0;   // arbitrary remesh edge length // move to inout commands

int load_surface_meshes(string in_dir, vector<Tissue> &tissues){
  path p (in_dir.c_str());
  try
  {
    if (!is_directory(p)){cout << p << "is not a directory\n";}
    else {
        cout << p << " is a directory containing:\n";
        typedef vector<path> vec;                                                                   // store and sort paths,
        vec v;
        copy(directory_iterator(p), directory_iterator(), back_inserter(v));
        sort(v.begin(), v.end());

        for (vec::const_iterator it (v.begin()); it != v.end(); ++it)                               // loop through input dir
        {
            cout << "it->filename() : " << it->filename().c_str() << "\n";                          // list files
            if(it->filename()=="tissues.yaml" ){                                                    // read "tissues.yaml" file
                std::ifstream input(it->c_str());
                cout << "\n Reading a yaml file \n\n";
                YAML::Node config = YAML::LoadFile(it->c_str());
                for (size_t i=0 ;i<config.size();i++){
                    cout << "Adding : ";
                    cout << config[i][0] << "\t";
                    cout << config[i][1] << "\t";
                    Tissue new_tissue;
                    new_tissue.name = config[i][0].as<string>();
                    new_tissue.YoungsModulus = config[i][1].as<int>();
                    std::vector<Mesh> temp_vec;
                    new_tissue.mesh_vec = temp_vec;
                    tissues.push_back(new_tissue);                                                  // push_back tissues
                    cout << "tissues.size() : "<< tissues.size() << "\n";
                }
                cout << "\n";
            }
        }
         
        cout << "\n\n" << p << " : Reading meshes\n\n";                                             // read *.off files 
        for (vec::const_iterator file_it (v.begin()); file_it != v.end(); ++file_it)
        {
            cout << "   " << *file_it << "\t\t";                                                       
            if(file_it->extension()==".off"){                                                       // if  *.off  load mesh
                std::ifstream input(file_it->c_str());
                Mesh  tmp_poly;
                input >> tmp_poly;
                                                                                                    // remesh to elimiate degenerate triangles  // bad idea: does not solve problem of later degenerate triangles from co-refinement.
               //CGAL::Polygon_mesh_processing::isotropic_remeshing( faces(tmp_poly), target_edge_length, tmp_poly);
                
                
                                                                                                                    //loop through tissues, boost::regex match filename to tissue
                for(std::vector<Tissue>::iterator tissue_it = tissues.begin(); tissue_it != tissues.end(); ++tissue_it) {
                    std::string s = file_it->filename().c_str();
                    std::string e = ".*"+ tissue_it->name +".*";
                    cout << "s = " << s << "\t e = " << e << "\n";
                    if(substring_match(e,s)){                                                       // if regex tissue[].name  match  filename  
                        cout << "pushing back\n";                                                   // caveat where one tissue_name contains another
                        tissue_it->mesh_vec.push_back(tmp_poly);                                    // then copy mesh to this tissue
                        cout << tissue_it->name << "->mesh_vec.size() = " << tissue_it->mesh_vec.size() << "\n";
                    }
                }
            }else{
                cout << "Not a .off file \t" << *file_it << "\n"; 
            }
            cout << "\n";
        }
        cout << "\n\n Finished reading meshed \n\n ";
    }
  }
    catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }
  
  cout << "tissues.size() = " << tissues.size() << "\n ";
  cout << "tissues.begin()->name = " << tissues.begin()->name << "\n ";
  cout << "tissues.begin()->mesh_vec.size() = " << tissues.begin()->mesh_vec.size() << "\n ";
//  cout << "tissues.begin()->mesh_vec.begin()->is_valid()  = " << tissues.begin()->mesh_vec.begin()->is_valid() << "\n ";

  return 0;
}
