# imports
import os
import bpy
from gport import g10_blender as g10

# data
D = bpy.data
s = g10.Scene(D.scenes[0])

# print the name
print(f'[gport] [scene] {s.name}')

# get the path
path : str = os.getenv('GPORT_PATH')

# error check
if path == None:

    # log
    print('Error: GPORT_PATH not set!\n')

    # abort
    os._exit(1)

# write the scene
s.write_to_file(f'{path}/scene/{s.name}.json')