
use <boat_propeller-nSpline_library/files/splines.scad>
use <boat_propeller-nSpline_library/files/Naca_sweep.scad> 


dat2=gen_segment_dat();
sweep(dat2);
//actuator_attachments(dat2, 0.1, 0.1, dat2, 0.3, 50);



function gen_segment_dat(
        M=20,   // number of facets around limb
        N=30,   // number of facets along length
        A=[ [10,5,0], [-10,5,0], [-10,-5,0], [10,-5,0], [10,5,0],] , // cross section data
        B=[ [0.5,0.25,0], [2,1,0], [1,2,0], [0.25,2,0], ] ,          // cross-section mod params
        C=[ [0,0,0], [10,1,50], [20,-5,100], [25,0,150], ]         // path data
    ) =
    let(cross_section = nSpline(A,50))
    let(path = nSpline(C,N))
    let(mod_params = nSpline(B,N))
    gen_sweep_dat(path,mod_params,A,M,N);


module actuator_attachments(segment1, s1longitude, s1latitude, segment2, s2longitude, actuator_length){ // segments = dat array of vertices, lat long as 0.0-1.0
    // origin 
    N1=len(segment1);
    M1=len(segment1[0]);
    origin_vertex = segment1[s1longitude*N1][s1latitude*M1];
    translate(origin_vertex) rotate([0,0,s1longitude*360])  cylinder(h=3,r1=1,r2=1.5);
    // insertion
    N2=len(segment2);
    M2=len(segment2[0]);
    s2latitude_index = 
        get_insertion_lat_index(
            segment2, s2longitude*M2,origin_vertex, actuator_length, 0);
    insertion_vertex = segment2[s2latitude_index][s2longitude*M2];
    translate(insertion_vertex) rotate([0,0,s1longitude*360]) color("blue") 
                                                    cylinder(h=3,r1=1,r2=1.5);
    }

function get_insertion_lat_index(dat,longindex,origin,actuator_len,i)=                      // recursive fn searches for 1st apex <= actuator_len
    let(distance = dist(origin,dat[i][longindex]) )
    (distance>actuator_len || i>60)?i:
        get_insertion_lat_index(dat,longindex,origin,actuator_len,i+1);

function dist(pointA=[0,0,0],pointB=[1,1,1]) =                                              // L2 distance between two points
    sqrt(
    pow(pointA[0]-pointB[0],2) + 
    pow(pointA[1]-pointB[1],2) + 
    pow(pointA[2]-pointB[2],2) 
    );

function normal(segment2, index_longitude, index_latitude) =                             // return normal vector of vertex
    let(N2= len(segment2))
    let(M2= len(segment2[0]))
    let(point0= segment2[index_longitude*N2][index_latitude*M2])
    let(point1= segment2[index_longitude*N2][index_latitude*M2-1])
    let(point2= segment2[index_longitude*N2-1][index_latitude*M2])
    [N2,M2,index_longitude, index_latitude,point0, point1, point2];

function gen_xSec(A,Bx,By) =                                                              // return points of x-section polygon, modified by B
   [ for (i=[0:len(A)-1]) 
       [ A[i][0]*Bx , A[i][1]*By, A[i][2] ]
   ];
   
function gen_sweep_dat(path,mod_params,A,M,N) = [                                     // return [array of polygons[arrayof vertices[x,y,z]]] 
   for(i=[0:N-1])
       let(perim=gen_xSec(nSpline(A,M),mod_params[i][0],mod_params[i][1]) )
        T_(path[i][0],path[i][1],path[i][2], perim)
   ];
  