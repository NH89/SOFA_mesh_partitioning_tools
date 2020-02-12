# Some notes on the formats of fies 

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

##### The tags _may_ need to be edited to declare the data as 32 bit 
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




<?xml version="1.0"?>
<VTKFile type="UnstructuredGrid" version="0.1" byte_order="BigEndian" >
  <UnstructuredGrid>
  <Piece NumberOfPoints="1336" NumberOfCells="6040">
    <Points>
      <DataArray type ="Float32" NumberOfComponents="3" format="ascii">
12.4933 4.04558 -14.9831 38.8089 -12.8525 5.99562 19.2022 -14.2921 12.5118 0.514315 27.9634 19.8861 11.5955 -14.3408 -3.18623 -6.25256 39.5787 16.2398 10.5438 
...
35.9775 6.97803 -9.95964 36.5654 -9.68745 -6.14162 8.1193 -1.59415 -8.12758 17.8133 23.9801 -6.18211 10.455 -2.15211 -9.57019 13.8217 19.2873 -0.279296 -3.30263 24.4518 -11.3255 
      </DataArray>
    </Points>
    <Cells>
      <DataArray type="Int32" Name="connectivity" format="ascii">
1317 1002 1117 459 1189 1075 84 1059 620 876 737 712 889 988 1238 1240 987 895 178 894 778 543 243 545 1007 738 821 953 793 1223 1052 683 1278 910 706 997 689 
...
1329 1090 652 848 721 1333 1331 689 1099 1334 1044 1334 1099 518 1044 1099 518 1164 1334 1099 1334 1164 689 
      </DataArray>
      <DataArray type="Int32" Name="offsets" format="ascii">
4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 124 128 132 136 140 144 148 152 156 160 164 168 172 176 180 184 188 
...
24108 24112 24116 24120 24124 24128 24132 24136 24140 24144 24148 24152 24156 24160 
      </DataArray>
      <DataArray type="UInt8" Name="types" format="ascii">
10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 
...
10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 
      </DataArray>
    </Cells>

   </Piece>
  </UnstructuredGrid>
</VTKFile>



```

#### The format of the .mesh file is:

	MeshVersionFormatted 1
	Dimension 3
	Vertices
	3688
	14.991330688524485 0.92362909848873365 -19.253197186051018 2
	40.090792840872666 -11.562952078188596 5.7180230530007545 1
	...
	10.34714061407881 5.8795302757934671 12.147978419248741 1
	27.776485721692524 34.791521065461495 -17.685294504580401 3
	Triangles
	12498
	1112 1808 2030 1
	1112 1808 2030 2
	...
	3676 3688 1230 3
	3676 3688 1230 4
	Tetrahedra
	16386
	2768 1112 1808 2030 2
	1731 1034 1114 1411 2
	3242 3502 3096 3593 1
	...
	1030 1031 3686 3625 1
	3117 57 627 1413 4
	End
	
Where 
- Vertices list x.y.z coordinates of nodes,
- Triangles list 3 nodes and the partition number,
- Tetrahedra list 4 nodes and the partition number.

The partition number is the order of precedence in the .yaml file.

To convert the partitioned tetrahedral mesh to .msh format:
Use the "gmsh" program (from your Linux repository, http://gmsh.info) to view partitioned tetrahedral mesh ".mesh" file, and save ".msh" file for Sofa :

    gmsh output_files2/out.mesh 

    ctrl-shift-S to save as .msh file for use in SOFA.


#### The format of the .msh file 
produced by Gmsh and ingested by SOFA is defined here 
http://gmsh.info/doc/texinfo/gmsh.html#MSH-file-format
and 
http://gmsh.info/doc/texinfo/gmsh.html#Legacy-formats

	$MeshFormat
	2.2 0 8
	$EndMeshFormat
	$Nodes
	3757
	1 40.09079284087267 -11.5629520781886 5.718023053000755
	2 21.83274017867465 -14.8626889686185 12.75549068535775
	...	
	3756 37.05858520334601 45.24848576497015 1.177002915023591
	3757 45.12630560735686 23.87858755229444 -0.7968016703654239
	$EndNodes
	$Elements
	29447
	1 2 2 0 1 462 425 423
	2 2 2 0 1 1269 1258 1266
	...
	12852 2 2 0 4 2349 2420 2294
	12853 4 2 0 1 3255 3586 3471 3483
	...
	29446 4 2 0 4 2179 2661 2253 2693
	29447 4 2 0 4 3726 2643 2709 2700
	$EndElements

Where
- Nodes list : node-number, x.y.z coordinates of nodes,
- Elements list : element-number, element type(2=triangle, 4=tetrahedron), 2,0, partition number, list of nodes


$Elements
  numEntityBlocks(size_t) numElements(size_t)
    minElementTag(size_t) maxElementTag(size_t)
  entityDim(int) entityTag(int) elementType(int; see below)
    numElementsInBlock(size_t)
    elementTag(size_t) nodeTag(size_t) ...
    ...
  ...
$EndElements

=======

gmsh-2 format:

    $Nodes
    number-of-nodes
    node-number x-coord y-coord z-coord
    …
    $EndNodes
    $Elements
    number-of-elements
    elm-number elm-type number-of-tags < tag > … node-number-list
    …
    $EndElements

    
    
