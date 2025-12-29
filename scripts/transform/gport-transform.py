# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data

# iterate through each object
for e in D.objects:

    # print the name
    print(f'[gport] [transform] {e.name}')

    # construct transforms
    xfrm = g10.Transform(e)

    # export the geometry as json
    print(xfrm.json())
    