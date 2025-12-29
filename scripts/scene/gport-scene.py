# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data
s = g10.Scene(D.scenes[0])

# print the name
print(f'[gport] [scene] {s.name}')

s.write_to_file(f'/Users/j/Desktop/g10/assets/input/scene/{s.name}.json')