# imports
import bpy
import gport

# data
D = bpy.data

# iterate through each object
for e in D.objects:

    # print the name
    print(f'[gport] [geometry] {e.name}')

    # construct geometry
    geom = gport.Geometry(e)

    # export the geometry as json
    geom.export_json('/Users/j/Desktop/g10/assets/input/'+e.name+'.json')
    