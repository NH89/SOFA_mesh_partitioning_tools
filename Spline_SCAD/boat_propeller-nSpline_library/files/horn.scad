////////////////////////////////////////////////////
// horn.scad - demo for mulivariate splines
// Implementation: Rudolf Huttary (c), Berlin 
//  Dezember 2015
//  commercial use prohibited

use <splines.scad>
use <Naca_sweep.scad> 

A = [//x,   y,z,      r, angle   // data outer skin
      [0,   0,0,      20,    0],
      [20,  0,200,    40,  -10],
      [60,  0,300,    70,  -60],
      [68,  0,310,    100, -62],
      [68+1,0,310+.6, 100, -62],
  ];

d = 1;  // thickness
A1 = [                           // data inner skin
  [-0.001,0,-0.001, 20-d, 0],    
  [20,    0,200,    40-d, -10],
  [60,    0,300,    70-d, -60],
  [68,    0,310,    100-d, -62],
  [68+1.1,0,310+.7, 100-d, -62],
  ];

B = nSpline(A,100);    // outer skin
B1 = nSpline(A1,100);  // inner skin

C = gen_dat(B, 100);   // generate data
C1 = gen_dat(B1, 100); // generate data

difference()
{
  sweep(C);
  sweep(C1);
}

function circle_(r, N) = [
  for(i=[0:N-1]) let(w = i*360/N)
    [r*sin(w), r*cos(w)]];
   
function gen_dat(S, N) =
   [ for (i=[0:len(S)-1])
       let(dat = Ry_(S[i][4], vec3D(circle_(S[i][3], N))))
        T_(S[i][0], S[i][1], S[i][2], dat)];
 
  