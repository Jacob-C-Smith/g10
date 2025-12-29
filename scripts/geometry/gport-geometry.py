# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data

# iterate through each object
for e in D.objects:

    # print the name
    print(f'[gport] [geometry] {e.name}')

    # construct geometry
    geom = g10.Geometry(e)

    # export the geometry as json
    geom.export_json(f'/Users/j/Desktop/g10/assets/input/geometry/{e.name}.json')