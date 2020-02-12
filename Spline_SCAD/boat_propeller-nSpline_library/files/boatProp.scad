use <Naca4.scad>      // see http://www.thingiverse.com/thing:898554
use <ShortCuts.scad>  // see http://www.thingiverse.com/thing:644830
use <Naca_sweep.scad> // see http://www.thingiverse.com/thing:900137
use <splines.scad>


// some parameters 
N = 100;         // refinement
R_axis = 100;   // radius of axle
r_bore = 50;    // radius of axle bore 
size = .1;      // sizing factor
slices = false;  // set true to view interpolation slices
ty = 50;        // common y-distancing of blades

// core data to be interpolated by nSpline()
//       transform data             airfoil nacaXYZZ  
//--------------------------------------------------
A = [ // Tx    Ty   Tz   Rz   Ry  |   L    X    ZZ
        [0,    ty,   60, -25,   0,  300,  .12,  .1],
        [-50,  ty,  200, -50,   0,  600,  .2,  .08],
        [-100, ty,  500, -75,   0,  700,  .1,  .06],
        [00,   ty,  660, -82,  10,   10,  .01,  .1],
    ]; 

// Main object
Rz(180)
S(size, size, size)
boat_prop(4); 

module boat_prop(M = 2)
{
  m = slices?1:M; // used for slices view
  $fn = N;        // refinement
  shift = -130;   // shift for axis parts
  shl = 270;      // length of axis
  B = nSpline(A, 2*N);  // interpolation
  
  D()  // difference
  {
    U()  // union of blades and axle
    {
      for(i=[0:360/m:359])
      {
        C = gen_dat(slices?A:B,N);
        R(i)                 // = Rx(); x-rotates object 
        sweep(C, showslices=slices); 
      }
        if(!slices)
          axis(shift, shl); 
    }
    Tx(shift+shl*.2)   // translate 
    Ry(90)             // rotate
      Cy(r_bore,shl*.9); // cylinder
  }
  
  // generate all data - applies transformation and airfoil params
  function gen_dat(B,N) = 
    [for(i=[0:len(B)-1])
      let(R = B[i])
       Tz_(R[2],   // z-translate 2D-shape
        R_(R[4], 0, 180+R[3], // rotate 2D-shape
          T_(R[0],R[1], 0, // xyz-translate 2D-shape
           vec3D(airfoil_data(N=N, L=R[5], naca=[R[6],.5,R[7]], open=true)
      ))))
    ];
}

module axis(shift = 0, shl = 200)
{
  T(shift)        // = Tx()
  Ry(90)
    Cy(R_axis,shl); 
  T(-shl/2+shift)
  Ry(90)
    Sp(R_axis);    // sphere
}