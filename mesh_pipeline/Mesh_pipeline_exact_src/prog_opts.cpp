#include "pipeline.hpp"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;
using namespace CGAL::parameters;

int prog_opts(int ac, char* av[], string * in_dir, string * out_dir, float * edge_size)
{
    string input_directory, output_directory;
    boost::tuple<int, int, int, int, int, int> m_criteria;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("input_dir,i", po::value<string>(), "pathname for input directory")
            ("output_dir,o", po::value<string>(), "pathname for output directory") 
            ("_edge_size,a", po::value<float>(), "edge_size")
            
            ("_facet_angle,b", po::value<float>(), "facet_angle")
            ("_facet_size,c", po::value<float>(), "facet_size")
            ("_facet_distance,d", po::value<float>(), "facet_distance")
            ("_cell_radius_edge_ratio,e", po::value<float>(), "cell_radius_edge_ratio")
            ("_cell_size,f", po::value<float>(), "cell_size")   
        ;
        float _a = 4;       // edge_size
        float _b = 25;      // facet_angle
        float _c = 4;       // facet_size
        float _d = 0.4;     // facet_distance
        float _e = 3;       // cell_radius_edge_ratio
        float _f = 4;       // cell_size
        
        int integer_val;
        integer_val =8;
        
        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    
        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }
        
        if (vm.count("input_dir")) {
            cout << "Input directory was set to " 
                 << vm["input_dir"].as<string>() << ".\n";
            input_directory = vm["input_dir"].as<string>();     
            *in_dir = input_directory;                          // copy to pointed location
        } else {
            cout << "Input directory was not set.\n";
        }
        
        if (vm.count("output_dir")) {
            cout << "Output directory was set to " 
                 << vm["output_dir"].as<string>() << ".\n";
            output_directory = vm["output_dir"].as<string>();     
            *out_dir = output_directory;                        // copy to pointed location
        } else {
            cout << "Output directory was not set.\n";
        }
        if (vm.count("_edge_size")) {
            cout << " was set to " << vm["_edge_size"].as<float>() << ".\n";
            *edge_size = vm["_edge_size"].as<float>();
        }
        if (vm.count("_facet_angle")) {
            cout << " was set to " << vm["_facet_angle"].as<float>() << ".\n";
            _b = vm["_facet_angle"].as<float>();
        }
        if (vm.count("_facet_size")) {
            cout << " was set to " << vm["_facet_size"].as<float>() << ".\n";
            _c = vm["_facet_size"].as<float>();
        }
        if (vm.count("_facet_distance")) {
            cout << " was set to " << vm["_facet_distance"].as<float>() << ".\n";
            _d = vm["_facet_distance"].as<float>();
        }
        if (vm.count("_cell_radius_edge_ratio")) {
            cout << " was set to " << vm["_cell_radius_edge_ratio"].as<float>() << ".\n";
            _e = vm["_cell_radius_edge_ratio"].as<float>();
        }
        if (vm.count("_cell_size")) {
            cout << " was set to " << vm["_cell_size"].as<float>() << ".\n";
            _f = vm["_cell_size"].as<float>();
        }

//         Mesh_criteria tmp_criteria(edge_size = _a,
//                          facet_angle = _b, facet_size = _c, facet_distance = _d,
//                          cell_radius_edge_ratio = _e, cell_size = _f);
//             /*
//                             edge_size,
//                             facet_angle,
//                             facet_size,
//                             facet_distance,
//                             cell_radius_edge_ratio,
//                             cell_size
//                             );*/
//         *mesh_criteria = tmp_criteria;
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    return 0;
}
