# imports
import bpy
from gport import g10_blender as g10

# data
D = bpy.data

# iterate through each object
for t in D.images:

    filter_mode = None
    addressing = None

    if t.type in {'RENDER_RESULT'}:
        continue

    file_ext = "png" 
    save_path = f'/Users/j/Desktop/g10/assets/input/texture/{t.name}.{file_ext}'
    
    # Save the image
    t.file_format = 'PNG'
    t.save_render(save_path)
    
    print(f'[gport] [texture] {t.name}')

