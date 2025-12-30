# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data

# iterate through each material
for m in D.materials:

    # print the name
    print(f'[gport] [material] {m.name}')

    # construct material
    mat = g10.Material(m)

    # export the material as json
    mat.write_to_file(f'/Users/j/Desktop/g10/assets/input/material/{m.name}.json')