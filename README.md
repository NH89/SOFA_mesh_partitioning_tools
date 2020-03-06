# README:  Partitioned mesh building tools

This repository contains tools to help scene designers to create partitioned meshes for use in multi-material finite element simulations.

SOFA allows the simulation of objects with parts composed of different materials, with different stiffnesses. 
These require partitioned tetrahedral meshes, such that groups of elements within a single mesh, can be assigned different properties. 

This may be useful for many applications, not limited to: anatomical simulations, soft robotics, games physics.


# Licenses:

The "Spline_SCAD/boat_propeller-nSpline_library" is subject to the CC-By-ND license from its original author - see "Spline_SCAD/boat_propeller-nSpline_library/LICENSE.txt"

The all other parts of this repository, including the "mesh_pipeline" and "test_mesh_generator" folders, and the Spline_SCAD/spline_organ.scad script, are copyright to CSIRO (my employer) and subject to the GNU Affero General Public License  http://www.gnu.org/licenses/agpl-3.0.html .

## Acknowledgement

This work was funded by the Commonwealth Scientific and Industrial Research Organisation (CSIRO) of Australia, as part of the Autonomous Design project within the Active Integrated Matter FSP
https://research.csiro.au/aim/home/aims-research-test-beds/autonomous-design/.

# Tools:
## 1  Mesh_pipeline
## 2  "test_mesh_generator_python2.py"
## 3  "spline_organ.scad"
## 4  Variable FEM stiffness via Sofa Python Controller



# 1  Mesh_pipeline
Mesh_pipeline takes as input a collection of **`.off`** surface meshes and a data file **`tissues.yaml`**, and outputs a **partitioned tetrahedral mesh** as both an ascii **`.vtu`** vtk xml file, and a **`.mesh`** file, plus **`.off`** files of the boundaries between the partitions.

The input meshes are triangulated surface meshes named <mesh_name><tissue_name>.off .
Mesh_pipeline combines (set union) all meshes of the same tissue_name.
The intersecting tissues are then subtracted from each other, in order of precedence of the tissue_name, to create the partitioned mesh.
The order of precedence of the tissue_name is given in the **`tissues.yaml`** file. 

e.g. in `input_files/input_files_demo/`   the files  `movable_sphere2_25_0_0_skin.off, sphere_0_10_skin.off, sphere_0_30_skin.off` will be 'union'ed to form a single 'skin' shape.
The 'skin' shape is first in precedence, so will be subtracted from the unions for each of the other tissues.


## *Warning*
Some meshes do cause problems generating tetrahedral meshes - probably due to excessively small and narrow triangles, e.g. the file input_files_dir/input_files_demo/sphere_0_0_s_kin.off .


## *Dependencies*
Mesh_pipeline is a c++ code using the CGAL arbitrary precision geometry library.
It depends on:
https://github.com/CGAL/cgal/tree/releases/CGAL-5.0-branch
yaml-cpp, gmp, mpfr, pthread
boost_regex, boost_program_options, boost_filesystem, boost_system,  boost_thread, boost_chrono, boost_date_time, boost_atomic, 

This version of the Mesh_pipeline is tested on Ubuntu 18.04, with CGAL 4.14.0 (installed in /usr/local), and boost-1.65-dev from the Ubuntu repository. 
(CGAL was built with the Eigen3 and Lapack options. Cmake may prompt you to install libraries and headers as needed.)

## Installing the dependencies

### On Linux
Use you package manager. e.g. on Debian based distributions such as Ubuntu:

    sudo apt-get install libyaml-cpp-dev libgmp-dev libmpfr-dev libpthread-stubs0-dev //
    libboost1.65-dev libboost_regex1.65-dev libboost_program_options1.65-dev //
    libboost_filesystem1.65-dev libboost_system1.65-dev  libboost_thread1.65-dev //
    libboost_chrono1.65-dev libboost_date_time1.65-dev libboost_atomic1.65-dev //
    libeigen3-dev lib liblapack-dev

### On Windows 10
The CGAL installer can download pre-compiled versions of GMP and MPFR.
The CGAL geometry library is here [CGAL Windows](https://www.cgal.org/download/windows.html)

You will need the [Boost binaries](https://sourceforge.net/projects/boost/files/boost-binaries/1.65.0/)
Boost [help/howto for Windows](https://www.boost.org/doc/libs/1_65_0/more/getting_started/windows.html)

yaml-cpp uses [C-Make](https://cmake.org), which you are probably already using if you have built SOFA. (If you haven't you will need to install it first. C-Make generates build scripts for your hardware & OS).
The yaml-cpp repository is [here](https://github.com/jbeder/yaml-cpp)

### On Mac
See [CGAL installation on Mac](https://www.cgal.org/download/macosx.html), and

[Boost installation on Mac](https://www.boost.org/doc/libs/1_65_0/doc/html/quickbook/install.html).

The yaml-cpp repository is [here](https://github.com/jbeder/yaml-cpp)


## Compile and link commands:

In  #~/projects/Mesh_pipeline$ 

    g++ -std=gnu++11 -c -g3 -fPIC prog_opts.cpp 
    g++ -std=gnu++11 -c -g3 -fPIC boost_regex_match.cpp  
    g++ -std=gnu++11 -c -g3 -fPIC load_surface_meshes.cpp
    g++ -std=gnu++11 -c -g3 -fPIC generate_subdomain_meshes.cpp
    g++ -std=gnu++11 -c -g3 -fPIC mkvec_cc2.cpp
    g++ -std=gnu++11 -c -g3 -fPIC make_partitioned_tet_mesh.cpp

    g++ -std=gnu++11 -g3 -fPIC -o pipeline2 pipeline2.cpp prog_opts.o load_surface_meshes.o generate_subdomain_meshes.o boost_regex_match.o mkvec_cc2.o make_partitioned_tet_mesh.o   -L/usr/local/lib -rdynamic -lboost_regex -lboost_program_options -lboost_filesystem -lboost_system -lyaml-cpp -lCGAL -lboost_thread -lgmp -lmpfr -lboost_chrono -lboost_date_time -lboost_atomic -lpthread -Wl,-rpath,/usr/local/lib



## Usage:

To generate test input data:

    #~/projects/test_mesh_generator_python$ python test_mesh_generator_python2.py 


Run "Meshpipeline" in ~/projects/Mesh_pipeline  with the command:

    ./pipeline2 -i ./input_files/ -o ./output_files

e.g.

	Mesh_pipeline_exact_src/pipeline2 -i input_files_dir/input_files_demo/  -o output_files_dir/output_files_demo

This should produce output identical to output_files_dir/output_files_demo_reference.




## "Mesh_pipeline"  Program options:

-   --help,                         produce help message

### Required options:
-   --input_dir,                -i, pathname for input directory
-   --output_dir,               -o, pathname for output directory 
 
### Mesh specification options (depend on the scale of the input meshes)
-   --_edge_size,               -a, edge_size
-   --_facet_angle,             -b, facet_angle
-   --_facet_size,              -c, facet_size
-   --_facet_distance,          -d, facet_distance
-   --_cell_radius_edge_ratio,  -e, cell_radius_edge_ratio
-   --_cell_size,               -f, cell_size


## Building a SOFA scene with a partitioned mesh:

Sofa allows a per-element modulus to be set for a tetrahedral mesh, see **Notes_on_the_SOFA_TetrahedronFEMForceField_class.md** and **Notes_on_the_SOFA_Python_API.md**.
This requires that you list the required moduli in the same order that the tetrahedral elements appear in the .msh file.

The file **`multi-stiffness_FEM_demo\demo.scn`**  shows how a partitioned mesh can be used to create a multi-material structue.

Note: local stiffnesses have been created from the partion numbers in the .vtu file, with find-replace 4->40. This produces one lobe that is clearly stiffer than the rest of the structure. See **`multi-stiffness_FEM_demo/Screenshot _paraview_partitions.png`** , the red lobe is partition 4, with local stiffness factor 40.



### Bug : Manual editing of .vtu file xml tags 64bit -> 32bit 

#### The format of the .vtu file is:

```
<?xml version="1.0"?>
<VTKFile type="UnstructuredGrid" version="0.1" byte_order="LittleEndian" header_type="UInt64">
  <UnstructuredGrid>
  <Piece NumberOfPoints="1336" NumberOfCells="6040">
    <Points>
      <DataArray type ="Float64" NumberOfComponents="3" format="ascii">
12.4933 4.04558 -14.9831 38.8089 -12.8525 5.99562 19.2022 -14.2921 12.5118 0.514315 27.9634 19.8861 11.5955 -14.3408 -3.18623 -6.25256 39.5787 16.2398 10.5438 45.4158 -6.59258 15.5472 -2.93167 17.3425 -17.0205 17.7197 -6.80648 -15.5557 24.1152 10.8482 -10.9982 -2.97701 
...
6.97803 -9.95964 36.5654 -9.68745 -6.14162 8.1193 -1.59415 -8.12758 17.8133 23.9801 -6.18211 10.455 -2.15211 -9.57019 13.8217 19.2873 -0.279296 -3.30263 24.4518 -11.3255 
      </DataArray>
    </Points>
    <Cells>
      <DataArray Name="connectivity" format="ascii" type="UInt64">
1317 1002 1117 459 1189 1075 84 1059 620 876 737 712 889 988 1238 1240 987 895 178 894 778 543 243 545 1007 738 821 953 793 1223 1052 
...
848 721 1333 1331 689 1099 1334 1044 1334 1099 518 1044 1099 518 1164 1334 1099 1334 1164 689 
      </DataArray>
      <DataArray Name="offsets" format="ascii" type="UInt64">
4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 124 128 132 136 140 144 148 152 156 160 
...
24128 24132 24136 24140 24144 24148 24152 24156 24160 
      </DataArray>
      <DataArray Name="types" format="ascii" type="UInt8">
10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 
...
10 10 10 10 10 10 10 10 10 10 
      </DataArray>
    </Cells>
    <CellData Scalars="MeshDomain">
      <DataArray type="Float64" Name="MeshDomain" format="ascii">
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 
...
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 
      </DataArray>
    </CellData>
   </Piece>
  </UnstructuredGrid>
</VTKFile>

```

#### The tags _may_ need to be edited to declare the data as 32 bit 
```
<VTKFile type="UnstructuredGrid" version="0.1" byte_order="LittleEndian" header_type="UInt64">
to 
<VTKFile type="UnstructuredGrid" version="0.1" byte_order="LittleEndian">


<DataArray type ="Float64" NumberOfComponents="3" format="ascii">
to
<DataArray type ="Float32" NumberOfComponents="3" format="ascii">


<DataArray Name="connectivity" format="ascii" type="UInt64">
to
<DataArray Name="connectivity" format="ascii" type="UInt32">


<DataArray Name="offsets" format="ascii" type="UInt64">
to
<DataArray Name="offsets" format="ascii" type="UInt32">


<DataArray type="Float64" Name="MeshDomain" format="ascii">
to
<DataArray type="Float32" Name="MeshDomain" format="ascii">
```


#### To use a multi-material partitioned mesh, see: 

- Notes_on_the_SOFA_TetrahedronFEMForceField_class.txt
- Notes_on_the_SOFA_Python_API.txt




# 2  "test_mesh_generator_python2.py"
This generates a set of test input meshes for Mesh_pipeline. 

NB the meshes are generated by calls to OpenSCAD.


# 3  "spline_organ.scad"
This is a script for OpenSCAD to generate swept spline surface meshes, which may be useful for modelling biological structures.
The script defines three splines from a set of control parameters 

- (i) the path to sweep, 
- (ii) a loop spline to sweep along the path, 
- (iii) a modifier spline that determines how the shape of the loop changes along the path.

## NB License
"spline_organ.scad" uses the **"boat_propellor-Spline_library"** included here. The **"boat_propellor-Spline_library"** is **CC-By_NC** licensed   Commercial users will neeed to obtain an appropriate licence, or remove the dependency.
The "boat_propellor-Spline_library" contains further examples of relevance to SOFA scene designers.


# 4  Variable FEM stiffness via Sofa Python Controller

The folder **`Variable_stiffness_FEM_demo/`** contains a demondstration of how to change the stiffness of a finite element mesh in a running SOFA simulation. These are derived from files in the **SOFA Robotis plugin**, but depend only on the **Sofa Python plugin**. 

Run the file **`Variable_stiffness_FEM_demo/Finger-edited_SparseLUSolver.pyscn`** in SOFA to test the demonstration.

#### Possible application
Changing FEM stiffness could be used to simulate mucular contraction. e.g. by having a low modlus FEM stretched by chaind of stif springs, to create a fibrous aniostropic 'resting' muscle. Stiffening of the FEM would cause contraction, returning the FEM to its un-streched length.




