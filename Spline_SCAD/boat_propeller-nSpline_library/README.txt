                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


https://www.thingiverse.com/thing:1208001
boat propeller customizable - OpenSCAD nSpline() library with show cases by Parkinbot is licensed under the Creative Commons - Attribution - Non-Commercial license.
http://creativecommons.org/licenses/by-nc/3.0/

# Summary

##nSpline
With this post I'm publishing my *splines* library. Its core function is **nSpline()**, which does an interpolation over a n-series of m-dimensional vectors each describing a 2D-shape, and calculates in-between values with arbitrary refinement.  

The progamming technique behind it is very general, so you might find it useful for your stuff as well.

For example you feed the function with seven 4D-vectors given as a 7x4 array and ask it to return you a 100x4 array. It will interpolate you each of the 4 dimensions over the series of 100 vectors. But you could also feed it with a 10x8 array and receive a 200x8 array with each (!) of the 8 dimensions interpolated. 
The m-dimensional vectors you can interpret along your needs, which means you have to provide a generator function doing that. Usally this function would use some part of such a vector to generate a parametrized 2D-shape and the other part to translate, rotate, scale, or even colorize this 2D-shape along some trajectory in 3D space. Just like a *linear_extrude()*, but with much more freedom - and at the price of much more self-responsibility.

For doing a sweep() upon any series of 2D shapes it is tremendously important that no polygon decribing a 2D shape self-intersects and no two 2D shapes within the series mutually intersect. Two 2D shapes may not even share a common vertex. OpenSCAD will *not* check the result of a sweep (which finally executes a polyhedron call) and currently even lets you export a non-manifold STL in some cases - so using this very mighty scheme is a bit like tightrope walking. 

I have tested it with great success for several projects, mostly in the field of blade design, but not only. I use it mainly for extrusion with my own [sweep()](http://www.thingiverse.com/thing:900137) function provided in my *Naca_Sweep* library. You can also use it without any change with the new [skin()](https://github.com/openscad/list-comprehension-demos) function currently evaluated as future language feature by the dev team.
```
// usage scheme - n-dimensional natural cubic spline interpolation 
  A = [[...],[...],[...]]; // define Mxm matrix, M>2, m>1
  B = nSplines(A, N);      // get interpolated Nxm matrix
  C = gen_dat(B);          // interpret data as sequence of 2D-shapes
                           // and generate trajectory
  sweep(B);                // extrude by knitting polyhedron object
```
As full blown examples to show the power of the approach I've designed 
- a customizable boat propeller in *boatprop.scad* that uses airfoil data and some realistic modelling of twist, camber and pitch. Its basic setup consists of 4 well placed slices only, as the *slices view* shown in the second image reveals - the 4th slice is too small to be visible. The parameters necessary to generate the 2D airfoil data for these four polygons make only a part of the primary data fed into nSpline(), the other part is used to describe the affine operations (translation and rotation) for 3D placement. The rich data set that results from interpolation is interpreted in the same way to generate a rich series of polygons, that can be swept into a full blown propeller blade with smooth surface transitions. 
- a horn in *horn.scad* as part of an instrument. The code uses a Boolean difference operation of an outer and an inner skin object. 

For further code examples see the more basic *knot()* example in the *splines.scad* file (slow CGAL-rendering(!)) and
 http://forum.openscad.org/Rendering-fails-difference-between-F5-and-F6-tp15041p15100.html
  http://forum.openscad.org/general-extrusion-imperfect-tp14740p14752.html

Extrusion functions offer you a lot of freedom, but also put responsibility on you
- you must avoid self-intersection of the extruded path. If self-intersection at some point is unavoidable you can try to split the array, sweep all parts separately and use a union().
- if you get assertion failures in CGAL rendering, check your design in *Thrown Together* view (F12) for wrongly (purple) colored triangles. In some cases you will have to just revert the vertex order of your 2D shapes, in others you might find self-intersections by closely examining the result in F5/F12 mode.