# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data

# iterate through each object
for e in D.objects:

    # print the name
    print(f'[gport] [entity] {e.name}')

    # type check
    if isinstance(e.data, bpy.types.Mesh) == False:
        continue

    # construct entity
    ent = g10.Entity(e)

    # export the entity as json
    ent.write_to_file(f'/Users/j/Desktop/g10/assets/input/entity/{e.name}.json')