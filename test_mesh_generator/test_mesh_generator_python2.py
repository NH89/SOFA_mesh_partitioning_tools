
import os
import subprocess
tissues = ["skin", "bone", "tendon", "ligament", "muscle", "loose_ct"]
steps = ["0","10","20","30"]
k=0
for i in steps:
    for j in steps:
        out_filename = "sphere_{}_{}_{}.off".format(i,j,tissues[k])
        openscad_cmd = "openscad -o{} -Dx={} -Dy={} -Dz=0 movable_sphere2.scad ".format(out_filename,i,j)   
        print(openscad_cmd.split())
        with open(out_filename, "w+") as f:
            subprocess.call(openscad_cmd.split())
            f.closed
    k = k+1
# nb add one rogue sphere to 'skin' to produce chalenging topology.
