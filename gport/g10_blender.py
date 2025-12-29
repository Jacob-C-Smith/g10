# 
# gport
#

# imports
import bpy, bmesh
import math, json, sys, time, os, getpass, importlib

from struct import pack

from dataclasses import dataclass

from timeit import default_timer as timer

# class declarations
## transform
class Transform:
    
    '''
        - Transform
    '''

    # fields
    location : list = None
    rotation : list = None
    scale : list = None

    json_data: dict = None

    # constructor
    def __init__(self, object: bpy.types.Object):

        # construct an empty dictionary
        self.json_data : dict = { }

        # location
        self.location = [ None, None, None ]
        self.location[0] = round(object.location[0], 3)
        self.location[1] = round(object.location[1], 3)
        self.location[2] = round(object.location[2], 3)

        # push the rotation mode
        temp = object.rotation_mode

        # set the rotation mode to xyz
        object.rotation_mode = 'XYZ'

        # rotation 
        self.rotation = [ None, None, None ]
        self.rotation[0] = (180.0/math.pi) * round(object.rotation_euler[0], 3)
        self.rotation[1] = (180.0/math.pi) * round(object.rotation_euler[1], 3)
        self.rotation[2] = (180.0/math.pi) * round(object.rotation_euler[2], 3)
        
        # pop the rotation mode
        object.rotation_mode = temp

        # scale
        self.scale = [ None, None, None ]
        self.scale[0] = round(object.scale[0], 3)
        self.scale[1] = round(object.scale[1], 3)
        self.scale[2] = round(object.scale[2], 3)

        # store the location
        self.json_data["location"] = self.location.copy()

        # store the rotation
        self.json_data["rotation"] = self.rotation.copy()

        # store the scale
        self.json_data["scale"] = self.scale.copy()

        # done
        return 

    # returns a json representation of a transform
    def json ( self ):

        # done
        return json.dumps(self.json_data, indent=4)
    
    # wriite the transform json to a file
    def export_json ( self, path ):

        # write the transform
        with open(path, "w") as f:
            try: json.dump(self.json_data, f, indent=4)
            except FileExistsError: pass

        # done
        return

## geometry
class Geometry:

    '''
        Geometry
    '''

    # fields
    name : str = None
    
    json_data : dict = None

    obj  : bpy.types.Object = None
    mesh : bpy.types.Mesh   = None

    path          : str  = None
    ply_path      : str  = None
    shader_name   : str  = None
    material_name : str  = None
    bone_data     : dict = None 

    # constructor
    def __init__(self, object: bpy.types.Object):

        # type check
        if isinstance(object.data, bpy.types.Mesh) == False:

            # done
            return

        # name
        self.name = object.data.name

        # blender mesh
        self.mesh = object

        # set up the dictionary
        self.json_data = { }
        self.json_data["name"] = self.name

        return

    # json exporter 
    def export_json ( self, path ):

        mesh = self.mesh.data

        # Ensure UVs exist
        if not mesh.uv_layers.active:
            print(f'[gport] [geometry] Failed to export \"{self.name}\"')
            
        # Calculate normals + tangents
        mesh.calc_normals_split()
        mesh.calc_tangents(uvmap=mesh.uv_layers.active.name)

        # Prepare material buckets
        mat_count = max(1, len(self.mesh.material_slots))
        bu = [[] for _ in range(mat_count)]

        loops = mesh.loops
        verts = mesh.vertices
        polys = mesh.polygons
        uv_layer = mesh.uv_layers.active.data

        mat_count = max(1, len(self.mesh.material_slots))

        geome = {"name":self.mesh.name,"xyz":[],"uv":[],"nxyz":[],"txyz":[],"parts":[]} 

        for i in range(mat_count):
            geome["parts"].append({"material":self.mesh.material_slots[i].name,"idx":[]})
            
        for poly in polys:
            mat_index = min(poly.material_index, mat_count - 1)
            loop_indices = poly.loop_indices

            for i in range(1, len(loop_indices) - 1):
                tri_loops = (
                    loop_indices[0],
                    loop_indices[i + 1],
                    loop_indices[i],
                )

                for li in tri_loops:
                    loop = loops[li]
                    vert = verts[loop.vertex_index]

                    co = vert.co
                    co = [round(co.x, 3), round(co.y, 3), round(co.z, 3)]

                    n = loop.normal
                    n = [n.x, n.y, n.z]

                    uv = uv_layer[li].uv
                    uv = [round(uv.x, 3), round(uv.y, 3)]

                    t = loop.tangent
                    sign = loop.bitangent_sign
                    t = [round(t.x, 3), round(t.y, 3), round(t.z,3), sign]

                    geome["xyz"].append(round(co[0], 3))
                    geome["xyz"].append(round(co[1], 3))
                    geome["xyz"].append(round(co[2], 3))
                    
                    geome["uv"].append(round(uv[0], 3))
                    geome["uv"].append(round(uv[1], 3))

                    geome["nxyz"].append(round(n[0], 3))
                    geome["nxyz"].append(round(n[1], 3))
                    geome["nxyz"].append(round(n[2], 3))
                    
                    geome["txyz"].append(round(t[0], 3))
                    geome["txyz"].append(round(t[1], 3))
                    geome["txyz"].append(round(t[2], 3))
                    geome["txyz"].append(round(t[3], 3))
                    
                    geome["parts"][mat_index]["idx"].append(li)


        print(f'writing to {path}')

        # write the geometry
        with open(path, "w") as f:
            try: json.dump(geome, f, indent=4)
            except FileExistsError: pass

        # done
        return 

## entity
class Entity:
    '''
        - Entity
    '''

    name     : str       = None
    transform : Transform = None

    json_data : dict = { }

    path : str = None

    def __init__(self, object: bpy.types.Object):

        # type check
        if isinstance(object.data, bpy.types.Mesh) == False:

            # empty -> name + transform
            if object.type == 'EMPTY':

                # set the `name
                self.name = object.name

                # construct the transform
                self.transform = Transform(object)

                # store the name of the empty
                self.json_data['name'] = self.name

                # store the transform
                if bool(self.transform.json_data):
                    self.json_data['transform'] = self.transform.json_data

                # done
                return
            
            # default
            else:
                
                # done
                return
        
        # set the name
        self.name = object.name
        
        # construct the transform
        self.transform = Transform(object)

        # store the name
        self.json_data['name'] = self.name

        # store the transform
        if bool(self.transform.json_data):
            self.json_data['transform'] = self.transform.json_data

        # if bool(self.geometry.json_data):
        self.json_data['geometry'] = f'/Users/j/Desktop/g10/assets/geometry/{self.name}.json'
        self.json_data['pipeline'] = 'color'

        # if bool(self.rigidbody.json_data):
        #     self.json_data['rigidbody'] = json.loads(self.rigidbody.json())

        # if bool(self.collider.json_data):
        #     self.json_data['collider'] = json.loads(self.collider.json())

        # if bool(object.parent):
        #     if isinstance(object.parent, bpy.types.Armature):
        #         self.rig = Rig(object.parent)
        #         self.json_data['rig'] = json.loads(self.rig.json())

        return
    
    def json (self):
        
        # done
        return (json.dumps(self.json_data, indent=4))

    def write_to_file(self, path: str):
        
        # write the json to the file
        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

        # done
        return

    def write_to_directory(self, directory: str):

        # Set the path to the entity json

        print(f"ENTITY WRITE TO DIRECTORY : {str(directory)} {str(self.name)}")
        self.path = directory + "/entities/" + self.name + ".json"

        # Directories for exports
        material_dir = directory + "/materials/"
        part_dir     = directory + "/part/"
        collider_dir = directory + "/collider/"

        # Save the textures
        if self.material is not None:
            self.material.save_textures(directory)
        
            # Write the material to a directory
            self.material.save_material(material_dir + self.material.name + ".json")
            self.json_data["materials"] = [ self.material.path ]
        
        if self.part is not None:
            # Save the part
            self.part.write_to_directory(directory)
            self.json_data["parts"]     = [ self.part.path ]
        
        # Return the JSON text
        return self.json()

        # Clean up
        if self.part is not None:
            del self.part
        if self.material is not None:
            del self.material

        return

## scene
class Scene:

    '''
        - Scene
    '''
    
    name : str = None
    
    entities : list = None
    cameras : list = None

    json_data : dict = None

    def __init__(self, scene: bpy.types.Scene):

        # Check for the right type
        if isinstance(scene, bpy.types.Scene) == False:
            # TODO: Throw an exception?
            return

        # set the name
        self.name = scene.name

        # Scene lists
        self.entities = []
        self.cameras = []
        self.json_data = { }

        # Scene JSON
        self.json_data["name"] = scene.name
        self.json_data["entities"] = []
        self.json_data["cameras"] = []
        
        # Iterate over each object in the scene
        for object in scene.objects:

            # light 
            if object.type == 'LIGHT':
                continue

            # camera
            elif object.type == 'CAMERA':
                
                # construct a camera
                c = Camera(object)

                # add the entity to the scene
                self.json_data["cameras"].append(c.json_data.copy())

            # entity
            elif object.type == 'MESH' or object.type == 'EMPTY':

                # construct an entity
                e = Entity(object)

                # add the entity to the scene
                self.json_data["entities"].append(e.json_data.copy())

            # Construct a light probe 
            elif object.type == 'LIGHT_PROBE':
                continue

            # Default case
            else:
                print("[gport] [scene] Unrecognized object in scene \"" + scene.name + "\"")
        
        # done    
        return

    def json(self):

        # done
        return json.dumps(self.json_data,indent=4)
    
    def write_to_file(self, path: str):
        
        # write the json to the file
        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

        # done
        return
    
    def write_to_directory(self, path: str):

        return

## light
class Light:

    '''
        - Light
    '''

    # Uninitialized data
    name      : str  = None
    location  : list = None
    color     : list = None
    light_type: str  = None

    json_data : dict = None

    # Constructor
    def __init__(self, object: bpy.types.Object):

        # Type check
        if isinstance(object.data, bpy.types.Light) == False:
            return

        # Set class data

        # Name
        self.name                  = object.name

        # Location
        self.location = [ None, None, None ]
        self.location[0]           = object.location[0]
        self.location[1]           = object.location[1]
        self.location[2]           = object.location[2]
        
        # Color
        self.color    = [ None, None, None ]
        self.color[0]              = object.data.color[0] * object.data.energy
        self.color[1]              = object.data.color[1] * object.data.energy
        self.color[2]              = object.data.color[2] * object.data.energy
        
        # Set up the dictionary
        self.json_data             = { }
        self.json_data["$schema"]  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/light-schema.json"
        self.json_data["name"]     = self.name
        self.json_data["location"] = self.location.copy()
        self.json_data["color"]    = self.color.copy()

        return

    # Returns file JSON
    def json(self):

        '''           
            Returns a G10 readable JSON object as a string
        '''

        # Dump the dictionary as a JSON object string
        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        ''' 
            Write a G10 readable JSON object text to a file path
        '''

        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

        return

    @staticmethod
    def import_from_file(path: str):
        
        # Uninitialized data
        light_json    : dict = None

        light_name    : str  = None
        light_location: list = None
        light_color   : list = None
        light_comment : str  = None
        
        # Open the light file from the path
        with open(path, "r") as f:

            # Make a dictionary from the JSON
            light_json = json.load(f)

        # Copy out important properties
        light_name     = light_json['name']
        light_location = light_json['location']
        light_color    = light_json['color']

        # Create a new light
        light_data = bpy.data.lights.new(name=light_name+" data", type='POINT')

        # Create a new object
        light_object = bpy.data.objects.new(name=light_name, object_data=light_data)
        
        # Add the new object to the current scene
        bpy.context.collection.objects.link(light_object)

        # Set the light properties
        light_object.location = light_location

## camera
class Camera:

    # fields
    name     : str   = None
    fov      : float = None
    near     : float = None
    far      : float = None
    target   : list  = None
    where    : list  = None
    
    json_data: dict  = None

    def __init__(self, object: bpy.types.Camera):

        # type check
        if isinstance(object.data, bpy.types.Camera) == False:
            return

        # set the name
        self.name = object.name

        # set the fov

        ## push the unit type
        tmp = object.data.lens_unit

        ## set the unit to fov
        object.data.lens_unit = 'FOV'

        ## store the fov
        self.fov = round(math.degrees(object.data.angle), 3)

        ## pop the unit type
        object.data.lens_unit = tmp

        # set the clippint plane
        self.near = round(object.data.clip_start, 3)
        self.far = round(object.data.clip_end, 3)

        # set the target
        self.target = [ None, None, None ]
        self.target[0] = round(object.matrix_world[0][2] * -1, 3)
        self.target[1] = round(object.matrix_world[1][2] * -1, 3)
        self.target[2] = round(object.matrix_world[2][2] * -1, 3)

        # set the up vector
        self.up = [ None, None, None ]
        self.up[0] = round(object.matrix_world[0][1], 3)
        self.up[1] = round(object.matrix_world[1][1], 3)
        self.up[2] = round(object.matrix_world[2][1], 3)

        # set the location
        self.where = [ None, None, None ]
        self.where[0] = round(object.matrix_world[0][3], 3)
        self.where[1] = round(object.matrix_world[1][3], 3)
        self.where[2] = round(object.matrix_world[2][3], 3)

        # set up the dictionary
        self.json_data             = { }
        self.json_data["name"]     = self.name
        self.json_data["fov"]      = self.fov
        self.json_data["location"] = (self.where.copy())
        self.json_data["up"]       = (self.up.copy())
        self.json_data["orientation"] = [
            round(self.where[0] + self.target[0], 3),
            round(self.where[1] + self.target[1], 3),
            round(self.where[2] + self.target[2], 3)
        ]
        self.json_data["clip"]     = {
            "near": self.near,
            "far" : self.far
        }

        # done
        return

    def json(self):

        # done
        return self.json_data

    # write json to a path 
    def write_to_file(self, path: str):
        
        # open the file
        with open(path, "w") as f:
            try: f.write(self.json())
            except FileExistsError: pass

        # done
        return

## texture
class Texture:
    '''
        - Texture
    '''

    json_data  : dict            = None

    image      : bpy.types.Image = None
    name       : str             = None
    path       : str             = None
    addressing : str             = 'repeat'
    filter_mode: str             = 'linear'
    generated  : bool            = None

    def __init__(self, *args):

        if len(args) > 1 or len(args) < 1:
            pass
        
        self.json_data = { }

        # Texture Image node
        if isinstance(args[0], bpy.types.ShaderNodeTexImage):
            
            if args[0].image is None:
                # TODO: Set missing texture
                print("Missing Texture")
                return
            image = args[0]

            # Set the image name
            self.name  = image.image.name

            # Set the image object
            self.image = image.image

            # Get the filter mode
            if   image.interpolation == 'Linear':
                self.filter_mode = 'linear'
            elif image.interpolation == 'Closest':
                self.filter_mode = 'nearest'

            # Get the addressing mode
            if   image.extension == 'REPEAT':
                self.addressing = 'repeat'
            elif image.extension == 'EXTEND':
                self.addressing = 'clamp edge'
            elif image.extension == 'CLIP':
                self.addressing = 'clamp border'

            self.generated = False

        # Image
        elif isinstance(args[0], bpy.types.Image):
            
            image = args[0]

            # Set the image name
            self.name = image.name
         
            # Set the image object
            self.image = image

            # Default to repeat addressing with linear filtering

            self.generated = True

        self.json_data['$schema']    = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/texture-schema.json"
        self.json_data['name']       = self.name
        
        self.json_data['addressing'] = self.addressing
        self.json_data['filter']     = self.filter_mode

        return

    # Save texture
    def save_texture(self,  path: str):
        self.path              = path
        self.json_data['path'] = self.path

        if self.image is not None:
            self.image.save_render(self.path)

        return
        
    # Returns JSON text of object
    def json(self):
        
        return json.dumps(self.json_data, indent=4)

    # Destructor
    def __del__(self):
        if self.image is not None:
            if self.generated is True:
                bpy.data.images.remove(self.image)

        return