# 
# GPort
#

import bpy, bmesh
import math, json, sys, time, os, getpass, importlib

from struct              import pack
from dataclasses         import dataclass
from timeit              import default_timer              as timer
from bpy_extras.io_utils import ExportHelper, ImportHelper
from bpy.types           import Operator, AddonPreferences

# TODO: Fix these, maybe use a json file on the disk to cache them?
materials      : dict = {}
entities       : dict = {}
parts          : dict = {}
g10_source     : dict = os.environ["G10_SOURCE_PATH"] if os.environ.get("G10_SOURCE_PATH") is not None else ""
export_context : dict = None

def set_export_context (context : dict):
    global export_context

    print(f"SET EXPORT CONTEXT STATE {context}")

    export_context = context


def clear_export_context ():
    global export_context
    
    export_context = None

class Light:

    '''
        gxport.Light
    '''

    # Uninitialized data
    name      : str  = None
    location  : list = None
    color     : list = None
    light_type: str  = None

    json_data : dict = None

    # Constructor
    def __init__(self, object: bpy.types.Object):

        '''
            Constructs a gxport.Light from a bpy.types.Object 
        '''

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

class Camera:

    '''
        - Camera
    '''

    name     : str   = None
    fov      : float = None
    near     : float = None
    far      : float = None
    target   : list  = None
    up       : list  = None
    where    : list  = None

    json_data: dict  = None

    def __init__(self, object: bpy.types.Camera):

        # Type check
        if isinstance(object.data, bpy.types.Camera) == False:
            return

        # Set the name
        self.name                = object.name

        # Set the FOV

        # Make a temporary variable for the unit type
        tmp                       = object.data.lens_unit

        object.data.lens_unit     = 'FOV'
        self.fov                  = object.data.lens

        # Restore the correct unit from the temp
        object.data.lens_unit     = tmp

        # Set the near clip
        self.near                 = object.data.clip_start

        # Set the far clip
        self.far                  = object.data.clip_end

        # Set the target
        self.target               = [ None, None, None ]
        self.target[0]            = object.matrix_world[0][2] * -1
        self.target[1]            = object.matrix_world[1][2] * -1
        self.target[2]            = object.matrix_world[2][2] * -1

        # Set the up 
        self.up                   = [ None, None, None ]
        self.up[0]                = object.matrix_world[0][1]
        self.up[1]                = object.matrix_world[1][1]
        self.up[2]                = object.matrix_world[2][1]

        # Set the location
        self.where                = [ None, None, None ]
        self.where[0]             = object.matrix_world[0][3]
        self.where[1]             = object.matrix_world[1][3]
        self.where[2]             = object.matrix_world[2][3]

        # Set up the dictionary
        self.json_data             = { }
        self.json_data["$schema"]  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/camera-schema.json"
        self.json_data["name"]     = self.name
        self.json_data["fov"]      = self.fov
        self.json_data["near"]     = self.near
        self.json_data["far"]      = self.far
        self.json_data["front"]    = (self.target.copy())
        self.json_data["up"]       = (self.up.copy())
        self.json_data["location"] = (self.where.copy())

        return

    def json(self):

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        ''' 
            Write a G10 readable JSON object text to a file path
        '''

        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

        return

    @staticmethod
    def import_as_json  (camera_json : dict):


        # Copy out important properties
        camera_name   : str   = camera_json['name']
        camera_where  : float = [ camera_json['location'][0], camera_json['location'][1], camera_json['location'][2] ]
        camera_target : float = [ camera_json['front'][0]   , camera_json['front'][1]   , camera_json['front'][2]    ]
        camera_up     : float = [ camera_json['up'][0]      , camera_json['up'][1]      , camera_json['up'][2]       ]
        camera_near   : list  = float(camera_json['near'])
        camera_far    : list  = float(camera_json['far'])
        camera_fov    : list  = float(camera_json['fov'])

        # Create a new camera
        camera_data = bpy.data.cameras.new(name=f"{camera_name} data")

        camera_data.clip_start = camera_near
        camera_data.clip_end   = camera_far
        
        # Make a temporary variable for the unit type
        camera_data.lens_unit     = 'FOV'
        camera_data.lens          = camera_fov

        # Create a new object
        camera_object = bpy.data.objects.new(name=camera_name, object_data=camera_data)
        
        # Add the new object to the current scene
        bpy.context.collection.objects.link(camera_object)

        # Set the camera properties
        camera_object.location = camera_where
        camera_object.matrix_world[0][1] = camera_up[0]
        camera_object.matrix_world[1][1] = camera_up[1]
        camera_object.matrix_world[2][1] = camera_up[2]

        camera_object.matrix_world[0][2] = camera_target[0] * -1
        camera_object.matrix_world[1][2] = camera_target[1] * -1
        camera_object.matrix_world[2][2] = camera_target[2] * -1

        return


    @staticmethod
    def import_from_file(path: str):
        
        # Uninitialized data
        camera_json   : dict  = None

        # Open the camera file from the path
        try: 
            with open(path, "r") as f:

                # Make a dictionary from the JSON
                camera_json = json.load(f)
        except e:
            print(e)

        Camera.import_as_json(camera_json)

class Geometry:

    '''
        Geometry
    '''

    name          : str              = None
    
    json_data     : dict             = None
    obj           : bpy.types.Object = None
    mesh          : bpy.types.Mesh   = None
    path          : str              = None
    ply_path      : str              = None
    shader_name   : str              = None
    material_name : str              = None
    bone_data     : dict             = None 

    # Constructor
    def __init__(self, object: bpy.types.Object):

        # Type check
        if isinstance(object.data, bpy.types.Mesh) == False:
            return

        # Set class data
        if len(object.material_slots) > 0:
            self.material_name         = object.material_slots[0].name
        
        # Name
        self.name                  = object.data.name

        # Blender mesh
        self.mesh = object

        # Set up the dictionary
        self.json_data             = { }
        self.json_data["$schema"]  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/part-schema.json"
        self.json_data["name"]     = self.name
        
        # global export_context
    
        # self.json_data["shader"]   = export_context['shader']
    
        if self.material_name is not None:
            self.json_data["material"] = self.material_name

        # Add the part to the cache
        parts[self.name] = self

        return

    # Returns file JSON
    def json(self):

        self.json_data['path'] = self.ply_path

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):

        self.json_data["path"] = self.ply_path

        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

    # PLY exporter 
    def export_json ( self, file_path, comment="" ):

        use_geometry    : bool  = True 
        use_uv_coords   : bool  = True 
        use_normals     : bool  = True 
        use_tangents    : bool  = True 
        use_bitangents  : bool  = True 

        # Make a new bmesh from the parameter
        bm = bmesh.new()
        bm.from_mesh(self.mesh.data)
        
        # Get UV layer
        uv_layer = bm.loops.layers.uv.active
        if uv_layer is None:
            use_uv_coords = False

        bmesh.ops.triangulate(bm, faces=bm.faces[:])

        # Buffer lookup tables
        bm.faces.ensure_lookup_table()
        bm.edges.ensure_lookup_table()
        bm.verts.ensure_lookup_table()

        # Dict for vertices and faces
        vertices = { }

        vertex_counter = 0

        faces = { }

        # Iterate over all faces (faces are triangulated)
        for i, f in enumerate(bm.faces):

            # Face vertex attributes
            t = [ 0.0, 0.0, 0.0 ]
            b = [ 0.0, 0.0, 0.0 ]

            # Face indicies
            face_indicies = [ 0, 0, 0 ]

            # Compute the tangent and bitangent of the face        
            if use_tangents is True or use_bitangents is True:

                # < x, y, z > coordinates for each vertex in the face
                pos1                = (f.verts[0].co)
                pos2                = (f.verts[1].co)
                pos3                = (f.verts[2].co)    

                # < s, t > coordinates for each vertex
                uv1 = f.loops[0][uv_layer].uv if use_uv_coords else [0.0, 0.0]
                uv2 = f.loops[1][uv_layer].uv if use_uv_coords else [0.0, 0.0]
                uv3 = f.loops[2][uv_layer].uv if use_uv_coords else [0.0, 0.0]

                # Compute the edges 
                edge1               = pos2 - pos1
                edge2               = pos3 - pos1

                # Compute the difference in UVs
                delta1              = uv2 - uv1
                delta2              = uv3 - uv1

                # Compute the inverse determinant
                det = float(delta1[0] * delta2[1] - delta2[0] * delta1[1])
                inverse_determinant = 1.0 / det if det != 0.0 else 0.0

                # Finally, construct the < tx, ty, tz > and < bx, by, bz > vectors
                t = [
                    inverse_determinant * float( delta2[1] * edge1[0] - delta1[1] * edge2[0] ),
                    inverse_determinant * float( delta2[1] * edge1[1] - delta1[1] * edge2[1] ),
                    inverse_determinant * float( delta2[1] * edge1[2] - delta1[1] * edge2[2] )
                ]

                b = [
                    inverse_determinant * float( -delta2[0] * edge1[0] + delta1[0] * edge2[0] ),
                    inverse_determinant * float( -delta2[0] * edge1[1] + delta1[0] * edge2[1] ),
                    inverse_determinant * float( -delta2[0] * edge1[2] + delta1[0] * edge2[2] )
                ]

                # Normalize t and b
                l_t = math.sqrt(t[0]*t[0] + t[1]*t[1] + t[2]*t[2])
                if l_t > 0:
                    t = [ t[0] / l_t, t[1] / l_t, t[2] / l_t ]

                l_b = math.sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2])
                if l_b > 0:
                    b = [ b[0] / l_b, b[1] / l_b, b[2] / l_b ]

            # Iterate over each vertex in the face
            for j, v in enumerate(f.verts):

                # Vertex attributes
                g  = [ None, None, None ]       # < x, y, z >
                uv = [ 0.0, 0.0 ]               # < s, t >
                n  = [ None, None, None ]       # < nx, ny, nz >
                
                # < x, y, z > of current vert
                if use_geometry is True:
                    g  = f.verts[j].co

                # < s, t > of current vert
                if use_uv_coords is True:
                    uv = f.loops[j][uv_layer].uv

                # < nx, ny, nz > of current vert
                if use_normals is True:
                    n  = f.verts[j].normal

                # Combine < x, y, z >
                #         < s, t >
                #         < nx, ny, nz >
                #         < tx, ty, tz >
                #         < bx, by, bz >

                combined_vertex = (
                                    g[0] , g[1] , g[2],         # 0  : < x, y, z >
                                    uv[0], uv[1],               # 3  : < s, t >
                                    n[0] , n[1] , n[2],         # 5  : < nx, ny, nz >
                                    t[0] , t[1] , t[2],         # 8  : < tx, ty, tz >
                                    b[0] , b[1] , b[2],         # 11 : < bx, by, bz >
                                  )

                index = vertices.get(combined_vertex)

                if index is not None:
                    face_indicies[j] = index
                else:            
                    face_indicies[j] = vertex_counter
                    vertices[combined_vertex] = vertex_counter

                    vertex_counter = vertex_counter + 1

            faces[i] = face_indicies

        # geometry
        ge = {
            "xyz" : [ ],
            "uv" : [ ],
            "nxyz" : [ ],
            "txyz" : [ ],
            "idx" : [ ]
        }

        # construct vertex data
        for v in vertices:

            # write < x, y, z >
            if use_geometry is True:
                ge["xyz"].append(v[0])
                ge["xyz"].append(v[1])
                ge["xyz"].append(v[2])

            # write < s, t >
            if use_uv_coords is True:
                ge["uv"].append(v[3])
                ge["uv"].append(v[4])

            # write < nx, ny, nz, nw >
            if use_normals is True:
                ge["nxyz"].append(v[5])
                ge["nxyz"].append(v[6])
                ge["nxyz"].append(v[7])

            # write < tx, ty, tz >
            if use_tangents is True:
                ge["txyz"].append(v[8])
                ge["txyz"].append(v[9])
                ge["txyz"].append(v[10])

                T = v[8:11]
                B = v[11:14]
                N = v[5:8]

                # compute chirality
                cx = N[1] * T[2] - N[2] * T[1]
                cy = N[2] * T[0] - N[0] * T[2]
                cz = N[0] * T[1] - N[1] * T[0]

                # compute sign
                sign = 1.0 if (cx * B[0] + cy * B[1] + cz * B[2]) > 0.0 else -1.0

                # store the tangent and the sign of the bitangent
                ge["txyz"].extend([T[0], T[1], T[2], sign])

        # round floats
        for key in ge:
            ge[key] = [float(f"{v:.6f}") for v in ge[key]]

        # construct index data
        for i, f in enumerate(faces):
            lf = faces[f] 
            ge["idx"].append(lf[0])
            ge["idx"].append(lf[1])
            ge["idx"].append(lf[2])

        # store the name
        ge["name"] = self.json_data["name"]

        # write the geometry
        with open(file_path, "w") as f:
            try: json.dump(ge, f, indent=4)
            except FileExistsError: pass

        return

    # This function gives me anxiety 
    def get_bone_groups_and_weights(self, object):

        bone_vertex_indices_and_weights = { }
        bone_group_array  = []
        bone_weight_array = []

        if len(object.vertex_groups) == 0:
            return None

        # Find vertex indices and bone weights for each bone    
        for g in object.vertex_groups:

            # Make a dictionary key for each bone
            bone_vertex_indices_and_weights[g.name] = []

            # And a convenience variable
            working_bone = bone_vertex_indices_and_weights[g.name]

            for v in object.data.vertices:
                for vg in v.groups:
                    if vg.group == g.index:
                        working_bone.append((v.index, vg.weight))

        for z in bone_vertex_indices_and_weights:
            print(z)

        '''
            bone_vertex_indices_and_weights now looks like
            {
                "bone name" : [ (1, 0.6), (2, 0.4), (3, 0.2), ... ],
                "spine"     : [ (9, 0.2), ... ],
                ...
                "head"      : [ ... , (3302, 0.23), (3303, 0.34), (3304, 0.6) ]
            }
        '''
        # This exporter only writes the 4 most heavily weighted bones to each vertex

        # Iterate over every vert
        for v in object.data.vertices:

            # Keep track of the 4 most heavy weights and their vertex groups
            heaviest_groups  = [ -1, -1, -1, -1 ] 
            heaviest_weights = [ 0, 0, 0, 0 ]

            # Iterate through bones
            for c in bone_vertex_indices_and_weights.keys():
                d = bone_vertex_indices_and_weights[c]

                for i in d:
                    if v.index == i[0]:
                        if i[1] > heaviest_weights[0]:
                            heaviest_groups[0]  = object.vertex_groups[c].index
                            heaviest_weights[0] = i[1]
                        elif i[1] > heaviest_weights[1]:
                            heaviest_groups[1]  = object.vertex_groups[c].index
                            heaviest_weights[1] = i[1]
                        elif i[1] > heaviest_weights[2]:
                            heaviest_groups[2]  = object.vertex_groups[c].index
                            heaviest_weights[2] = i[1]
                        elif i[1] > heaviest_weights[3]:
                            heaviest_groups[3]  = object.vertex_groups[c].index
                            heaviest_weights[3] = i[1]
                        else:
                            None
            bone_group_array.append(heaviest_groups)
            bone_weight_array.append(heaviest_weights)

        return (bone_group_array, bone_weight_array)

    # Get bone names and vertex group indicies
    def get_bone_names_and_indexes(self, object):

        ret: dict = { }

        for vg in object.vertex_groups:
            ret[vg.name] = vg.index

        return ret

    # Writes JSON and ply to a directory 
    def write_to_directory(self, directory: str):

        parts_directory = directory + "/parts/"

        self.ply_path   = (parts_directory + self.name + ".ply")
        self.path       = (parts_directory + self.name + ".json")

        temp_mat = self.mesh.matrix_world.copy()
        self.export_json(self.ply_path, "")
        self.mesh.matrix_world = temp_mat

        self.write_to_file(self.path)
        
        return

class Part:

    name          : str              = None
    
    json_data     : dict             = None
    obj           : bpy.types.Object = None
    mesh          : bpy.types.Mesh   = None
    path          : str              = None
    ply_path      : str              = None
    shader_name   : str              = None
    material_name : str              = None
    bone_data     : dict             = None 

    # Constructor
    def __init__(self, object: bpy.types.Object):

        # Type check
        if isinstance(object.data, bpy.types.Mesh) == False:
            return

        # Set class data
        if len(object.material_slots) > 0:
            self.material_name         = object.material_slots[0].name
        
        # Name
        self.name                  = object.data.name

        # Blender mesh
        self.mesh = object

        # Set up the dictionary
        self.json_data             = { }
        self.json_data["$schema"]  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/part-schema.json"
        self.json_data["name"]     = self.name
        
        global export_context
    
        self.json_data["shader"]   = export_context['shader']
    
        if self.material_name is not None:
            self.json_data["material"] = self.material_name

        # Add the part to the cache
        parts[self.name] = self

        return

    # Returns file JSON
    def json(self):

        self.json_data['path'] = self.ply_path

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):

        self.json_data["path"] = self.ply_path

        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try: f.write(self.json())
            except FileExistsError: pass

    # PLY exporter 
    def export_ply ( self, file_path, comment="Written from gxport" ):

        # Convinience 
        active_uv_layer         = self.mesh.data.uv_layers.active.data
        active_col_layer        = None
        bone_groups_and_weights = None
        bone_groups             = None
        bone_weights            = None

        use_geometry    : bool  = True if 'xyz'  in export_context['vertex groups'] else False
        use_uv_coords   : bool  = True if 'uv'   in export_context['vertex groups'] else False
        use_normals     : bool  = True if 'nxyz' in export_context['vertex groups'] else False
        use_tangents    : bool  = True if 'txyz' in export_context['vertex groups'] else False
        use_bitangents  : bool  = True if 'bxyz' in export_context['vertex groups'] else False
        use_colors      : bool  = True if 'rgb'  in export_context['vertex groups'] else False
        use_bone_groups : bool  = True if 'bg'   in export_context['vertex groups'] else False
        use_bone_weights: bool  = True if 'bw'   in export_context['vertex groups'] else False

        if use_colors is True:
            active_col_layer = self.mesh.vertex_colors.active.data

        # Make a new bmesh from the parameter
        bm = bmesh.new()
        bm.from_mesh(self.mesh.data)
        bmesh.ops.triangulate(bm, faces=bm.faces[:])

        # Buffer lookup tables
        bm.faces.ensure_lookup_table()
        bm.edges.ensure_lookup_table()
        bm.verts.ensure_lookup_table()

        # Dict for vertices and faces
        vertices = { }

        vertex_counter = 0

        faces   = { }

        if use_bone_groups is True or use_bone_weights is True:
            bone_groups_and_weights = self.get_bone_groups_and_weights(self.mesh)
            bone_groups  = bone_groups_and_weights[0]
            bone_weights = bone_groups_and_weights[1]


        # Iterate over all faces (faces are triangulated)
        for i, f in enumerate(bm.faces):

            # Face vertex attributes
            t             = [ None, None, None ]
            b             = [ None, None, None ]

            # Face indicies
            face_indicies = [ 0, 0, 0 ]

            # Compute the tangent and bitangent of the face        
            if use_tangents is True or use_bitangents is True:

                # < x, y, z > coordinates for each vertex in the face
                pos1                = (f.verts[0].co)
                pos2                = (f.verts[1].co)
                pos3                = (f.verts[2].co)    

                # < s, t > coordinates for each vertex
                uv1                 = active_uv_layer[f.loops[0].index].uv
                uv2                 = active_uv_layer[f.loops[1].index].uv
                uv3                 = active_uv_layer[f.loops[2].index].uv

                # Compute the edges 
                edge1               = pos2 - pos1
                edge2               = pos3 - pos1

                # Compute the difference in UVs
                delta1              = uv2 - uv1
                delta2              = uv3 - uv1

                # Compute the inverse determinant
                inverse_determinant = float(1) / float(delta1[0] * delta2[1] - delta2[0] * delta1[1])

                # Finally, construct the < tx, ty, tz > and < bx, by, bz > vectors
                t = [
                    inverse_determinant * float( delta2[1] * edge1[0] - delta1[1] * edge2[0] ),
                    inverse_determinant * float( delta2[1] * edge1[1] - delta1[1] * edge2[1] ),
                    inverse_determinant * float( delta2[1] * edge1[2] - delta1[1] * edge2[2] )
                ]

                b = [
                    inverse_determinant * float( -delta2[0] * edge1[0] + delta1[0] * edge2[0] ),
                    inverse_determinant * float( -delta2[0] * edge1[1] + delta1[0] * edge2[1] ),
                    inverse_determinant * float( -delta2[0] * edge1[2] + delta1[0] * edge2[2] )
                ]

                # Normalize t and b
                l_t = math.sqrt(t[0]*t[0] + t[1]*t[1] + t[2]*t[2])
                if l_t > 0:
                    t = [ t[0] / l_t, t[1] / l_t, t[2] / l_t ]

                l_b = math.sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2])
                if l_b > 0:
                    b = [ b[0] / l_b, b[1] / l_b, b[2] / l_b ]

            # Iterate over each vertex in the face
            for j, v in enumerate(f.verts):

                # Vertex attributes
                g  = [ None, None, None ]       # < x, y, z >
                uv = [ None, None ]             # < s, t >
                n  = [ None, None, None ]       # < nx, ny, nz >
                c  = [ None, None, None, None ] # < r, g, b, a >
                bg = [ None, None, None, None ] # < g0, g1, g2, g3 > 
                bw = [ None, None, None, None ] # < w0, w1, w2, w3 >

                # < x, y, z > of current vert
                if use_geometry is True:
                    g  = f.verts[j].co

                # < s, t > of current vert
                if use_uv_coords is True:
                    uv = active_uv_layer[f.loops[j].index].uv

                # < nx, ny, nz > of current vert
                if use_normals is True:
                    n  = f.verts[j].normal

                # < r, g, b, a >
                if use_colors is True:
                    c  = active_col_layer[f.loops[j].index].color

                # TODO: Bone groups and weights
                if use_bone_groups is True:
                    bg = bone_groups[f.verts[j].index]

                if use_bone_weights is True:
                    bw = bone_weights[f.verts[j].index]              

                # Combine < x, y, z >
                #         < s, t >
                #         < nx, ny, nz >
                #         < tx, ty, tz >
                #         < bx, by, bz >
                #         < r, g, b, a >
                #         < g0, g1, g2, g3 >
                #         < w0, w1, w2, w3 >

                combined_vertex = (
                                    g[0] , g[1] , g[2],         # 0  : < x, y, z >
                                    uv[0], uv[1],               # 3  : < s, t >
                                    n[0] , n[1] , n[2],         # 5  : < nx, ny, nz >
                                    t[0] , t[1] , t[2],         # 8  : < tx, ty, tz >
                                    b[0] , b[1] , b[2],         # 11 : < bx, by, bz >
                                    c[0] , c[1] , c[2] , c[3],  # 14 : < r, g, b, a >
                                    bg[0], bg[1], bg[2], bg[3], # 18 : < g0, g1, g2, g3 >
                                    bw[0], bw[1], bw[2], bw[3], # 22 : < w0, w1, w2, w3 >
                                  )

                index = vertices.get(combined_vertex)

                if index is not None:
                    face_indicies[j] = index
                else:            
                    face_indicies[j] = vertex_counter
                    vertices[combined_vertex] = vertex_counter

                    vertex_counter = vertex_counter + 1

            faces[i] = face_indicies

        with open(file_path, "wb") as file:
            fw = file.write

            fw(b"ply\n")
            fw(b"format binary_little_endian 1.0\n")

            if comment is not None:
                fw(b"comment " + bytes(comment, 'ascii') + b"\n")

            fw(b"element vertex %d\n" % vertex_counter)

            if use_geometry is True:
                fw(
                    b"property float x\n"
                    b"property float y\n"
                    b"property float z\n"
                )
                
            if use_uv_coords is True:
                fw(
                    b"property float s\n"
                    b"property float t\n"
                )
            if use_normals is True:
                fw(
                    b"property float nx\n"
                    b"property float ny\n"
                    b"property float nz\n"
                )
            if use_tangents is True:
                fw(
                    b"property float tx\n"
                    b"property float ty\n"
                    b"property float tz\n"
                )
            if use_bitangents is True:
                fw(
                    b"property float bx\n"
                    b"property float by\n"
                    b"property float bz\n"
                )
            if use_colors is True:
                fw(
                    b"property uchar red\n"
                    b"property uchar green\n"
                    b"property uchar blue\n"
                    b"property uchar alpha\n"
                )
            if use_bone_groups is True:
                fw(
                    b"property uchar b0\n"
                    b"property uchar b1\n"
                    b"property uchar b2\n"
                    b"property uchar b3\n"
                )
            if use_bone_weights is True:
                fw(
                    b"property uchar w0\n"
                    b"property uchar w1\n"
                    b"property uchar w2\n"
                    b"property uchar w3\n"
                )

            fw(b"element face %d\n" % len(faces))
            fw(b"property list uchar uint vertex_indices\n")
            fw(b"end_header\n")

            # Iterate over vertices
            for v in vertices:

                # Write < x, y, z >
                if use_geometry is True:
                    fw(pack("<3f", v[0] , v[1], v[2] ))

                # Write < s, t >
                if use_uv_coords is True:
                    fw(pack("<2f", v[3] , v[4] ))

                # Write < nx, ny, nz >
                if use_normals is True:
                    fw(pack("<3f", v[5] , v[6] , v[7] ))

                # Write < tx, ty, tz >
                if use_tangents is True:
                    fw(pack("<3f", v[8] , v[9] , v[10]))

                # Write < bx, by, bz >
                if use_bitangents is True:
                    fw(pack("<3f", v[11], v[12], v[13]))

                # Write < r, g, b, a >
                if use_colors is True:
                    fw(pack("<4B", v[14], v[15], v[16], v[17]))

                if use_bone_groups is True:
                    fw(pack("<4i", v[18], v[19], v[20], v[21]))


                if use_bone_weights is True:
                    fw(pack("<4f", v[22], v[23], v[24], v[25]))

            # Iterate over faces
            for i, f in enumerate(faces):
                w = "<3I"
                fw(pack("<b", 3))
                lf = faces[f] 
                fw(pack(w, lf[0], lf[1], lf[2]))

        return     

    # This function gives me anxiety 
    def get_bone_groups_and_weights(self, object):

        bone_vertex_indices_and_weights = { }
        bone_group_array  = []
        bone_weight_array = []

        if len(object.vertex_groups) == 0:
            return None

        # Find vertex indices and bone weights for each bone    
        for g in object.vertex_groups:

            # Make a dictionary key for each bone
            bone_vertex_indices_and_weights[g.name] = []

            # And a convenience variable
            working_bone = bone_vertex_indices_and_weights[g.name]

            for v in object.data.vertices:
                for vg in v.groups:
                    if vg.group == g.index:
                        working_bone.append((v.index, vg.weight))

        for z in bone_vertex_indices_and_weights:
            print(z)

            # bone_vertex_indices_and_weights now looks like
            # {
            #     "bone name" : [ (1, 0.6), (2, 0.4), (3, 0.2), ... ],
            #     "spine"     : [ (9, 0.2), ... ],
            #     ...
            #     "head"      : [ ... , (3302, 0.23), (3303, 0.34), (3304, 0.6) ]
            # }
        # This exporter only writes the 4 most heavily weighted bones to each vertex

        # Iterate over every vert
        for v in object.data.vertices:

            # Keep track of the 4 most heavy weights and their vertex groups
            heaviest_groups  = [ -1, -1, -1, -1 ] 
            heaviest_weights = [ 0, 0, 0, 0 ]

            # Iterate through bones
            for c in bone_vertex_indices_and_weights.keys():
                d = bone_vertex_indices_and_weights[c]

                for i in d:
                    if v.index == i[0]:
                        if i[1] > heaviest_weights[0]:
                            heaviest_groups[0]  = object.vertex_groups[c].index
                            heaviest_weights[0] = i[1]
                        elif i[1] > heaviest_weights[1]:
                            heaviest_groups[1]  = object.vertex_groups[c].index
                            heaviest_weights[1] = i[1]
                        elif i[1] > heaviest_weights[2]:
                            heaviest_groups[2]  = object.vertex_groups[c].index
                            heaviest_weights[2] = i[1]
                        elif i[1] > heaviest_weights[3]:
                            heaviest_groups[3]  = object.vertex_groups[c].index
                            heaviest_weights[3] = i[1]
                        else:
                            None
            bone_group_array.append(heaviest_groups)
            bone_weight_array.append(heaviest_weights)

        return (bone_group_array, bone_weight_array)

    # Get bone names and vertex group indicies
    def get_bone_names_and_indexes(self, object):

        ret: dict = { }

        for vg in object.vertex_groups:
            ret[vg.name] = vg.index

        return ret

    # Writes JSON and ply to a directory 
    def write_to_directory(self, directory: str):

        parts_directory = directory + "/parts/"

        self.ply_path   = (parts_directory + self.name + ".ply")
        self.path       = (parts_directory + self.name + ".json")

        temp_mat = self.mesh.matrix_world.copy()
        self.export_ply(self.ply_path, "")
        self.mesh.matrix_world = temp_mat

        self.write_to_file(self.path)
        
        return

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

class Material:

    '''
        - Material
    '''

    json_data: dict = None

    name:      str  = None

    path:      str  = None

    principled_node: bpy.types.ShaderNodeBsdfPrincipled = None
    node_tree: bpy.types.ShaderNodeTree = None

    albedo_node = None
    rough_node  = None
    metal_node  = None
    normal_node = None
    
    albedo: Texture = None
    rough : Texture = None
    metal : Texture = None
    normal: Texture = None
    ao    : Texture = None
    height: Texture = None

    def __init__(self, material: bpy.types.Material):

         # Set the node tree
        self.node_tree = material.node_tree

        self.name      = material.name

        self.json_data = { }

        # Right now, only principled BSDF is supported
        if self.node_tree.nodes.find('Principled BSDF') != -1:    
            
            self.principled_node = self.node_tree.nodes["Principled BSDF"]
            
            if isinstance(self.principled_node, bpy.types.ShaderNodeBsdfPrincipled) == False:
                print("ERROR NO PRINCIPLED NODE")
                return
        else:
            print("ERROR ONLY SUPPORTS PRINCIPLED BSDF" + material.name)
            return
        
        #################
        # Export albedo #
        #################

        # Set the albedo node
        self.albedo_node = self.principled_node.inputs["Base Color"]

        # Are there links on the node?
        if bool(self.albedo_node.links):

            # This branch is for exporting images
            if isinstance(self.albedo_node.links[0].from_node, bpy.types.ShaderNodeTexImage):
                self.albedo = Texture(self.albedo_node.links[0].from_node)

            # This branch is for baking images
            else:
                print("LINKED TO NODE SETUP")
        
        # If there are no links, make a new 1x1 image with the color in "Base Color"
        else:
            c = self.albedo_node.default_value
            bpy.ops.image.new(name=self.name + " albedo", width=1, height=1, color=(c[0], c[1], c[2], 1.0), alpha=False, generated_type='BLANK', float=True)
            self.albedo = Texture(bpy.data.images[self.name + " albedo"])
        
        ################
        # Export rough #
        ################
        
        # Set the rough node
        self.rough_node = self.principled_node.inputs["Roughness"]

        # Are there any links on the roughness input?
        if bool(self.rough_node.links):

            # Export an image
            if isinstance(self.rough_node.links[0].from_node, bpy.types.ShaderNodeTexImage):
                self.rough = Texture(self.rough_node.links[0].from_node)

            # Bake an image
            else:
                print("LINKED TO NODE SETUP")
        
        # If there are no links, make a new 1x1 image with the color in "Roughness"
        else:
            c = self.rough_node.default_value
            bpy.ops.image.new(name=self.name+" rough", width=1, height=1, color=(c,c,c,1.0), alpha=False, generated_type='BLANK', float=True)
            self.rough = Texture(bpy.data.images[self.name+' rough'])
        
        ################
        # Export metal #
        ################

        # Set the metal node
        self.metal_node = self.principled_node.inputs["Metallic"]

        # Are there any links on the metalness input?
        if bool(self.metal_node.links):

            # Export an image
            if isinstance(self.metal_node.links[0].from_node, bpy.types.ShaderNodeTexImage):
                self.metal = Texture(self.metal_node.links[0].from_node)

            # Bake an image
            else:
                print("LINKED TO NODE SETUP")
        
        # If there are no links, make a new 1x1 image with the color in "Metalness"
        else:
            c = self.metal_node.default_value
            bpy.ops.image.new(name=self.name+" metal", width=1, height=1, color=(c,c,c,1.0), alpha=False, generated_type='BLANK', float=True)
            self.metal = Texture(bpy.data.images[self.name+' metal'])
        
        #################
        # Export normal #
        #################

        # Set the normal node
        self.normal_node = self.principled_node.inputs["Normal"]

        # Are there any links on the normal input?
        if bool(self.normal_node.links):

            # Export an image
            if isinstance(self.normal_node.links[0].from_node, bpy.types.ShaderNodeTexImage):
                self.normal = Texture(self.normal_node.links[0].from_node)

            # Bake an image
            else:
                print("LINKED TO NODE SETUP")
        
        #############
        # Export AO #
        #############
        
        #################
        # Export Height #
        #################
        


        self.json_data['$schema']  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/material-schema.json"
        self.json_data['name']     = material.name
        self.json_data['textures'] = []

        materials[material.name] = self

        return

    # Bake images
    def bake(self, path: str):

        return

    def bake_albedo(self):
        pass
    def bake_rough(self):
        pass
    def bake_metal(self):
        pass
    def bake_normal(self):
        pass
    def bake_ao(self):
        pass
    def bake_height(self):
        pass

    # Save all textures
    def save_textures(self, directory: str):

        # Construct a texture
        texture_directory: str = directory + "/textures/" + self.name 

        global export_context

        # Make a directory for the textures
        try:    os.mkdir(texture_directory)
        except: pass

        # Save the albedo texture
        if 'albedo' in export_context['material textures']:
            if self.albedo is not None:
                self.albedo.save_texture(texture_directory + "/albedo." + "png")

        # Save the rough texture
        if 'rough' in export_context['material textures']:
            if self.rough is not None:
                self.rough.save_texture(texture_directory + "/rough." + "png")

        # Save the metal texture
        if 'metal' in export_context['material textures']:
            if self.metal is not None:
                self.metal.save_texture(texture_directory + "/metal." + "png")

        # Save the normal texture
        if 'normal' in export_context['material textures']:
            if self.normal is not None:
                self.normal.save_texture(texture_directory + "/normal." + "png")

        # Save the ambient occlusion texture
        if 'ao' in export_context['material textures']:
            if self.ao is not None:
                self.ao.save_texture(texture_directory + "/ao." + "png")

        # Save the height texture
        if 'height' in export_context['material textures']:
            if self.height is not None:
                self.height.save_texture(texture_directory + "/height." + "png")

        return

    # Save each material texture to a directory
    def save_material(self,  path: str):
        
        self.path              = path
        self.json_data['path'] = self.path
        self.json_data['textures'] = []
        
        if 'albedo' in export_context['material textures']:
            if self.albedo:
                self.json_data['textures'].append(json.loads(self.albedo.json()))
        if 'rough' in export_context['material textures']:
            if self.rough:
                self.json_data['textures'].append(json.loads(self.rough.json()))
        if 'metal' in export_context['material textures']:
            if self.metal:
                self.json_data['textures'].append(json.loads(self.metal.json()))
        if 'normal' in export_context['material textures']:
            if self.normal:
                self.json_data['textures'].append(json.loads(self.normal.json()))
        if 'ao' in export_context['material textures']:
            if self.ao:
                self.json_data['textures'].append(json.loads(self.ao.json()))
        if 'height' in export_context['material textures']:
            if self.height:
                self.json_data['textures'].append(json.loads(self.height.json()))

        self.write_to_file(self.path)

        return

    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

    # Returns JSON text of object
    def json(self):

        return json.dumps(self.json_data, indent=4)

    # Destructor
    def __del__(self):

        # Delete all the textures
        if self.albedo is not None:
            del(self.albedo)

        if self.rough is not None:
            del(self.rough)

        if self.metal is not None:
            del(self.metal)

        if self.normal is not None:
            del(self.normal)

        if self.ao is not None:
            del(self.ao)

        if self.height is not None:
            del(self.height)
        
        return

class LightProbe:

    '''
        - Light Probes
    '''

    json_data: dict = None

    def __init__(self, object: bpy.types.Object):
        self.json_data = { }

        # Spawn a very small sphere at the location of the light probe
        bpy.ops.mesh.primitive_uv_sphere_add(segments=64, ring_count=32, radius=0.001, calc_uvs=True, enter_editmode=False, align='WORLD', location=(object.location[0], object.location[1], object.location[2]), rotation=(0, 0, 0), scale=(1, 1, 1))

        pass

    # Bake a scaled down sphere with an equirectangular UV? Maybe use a cubemap?
    pass

class Transform:
    
    '''
        - Transform
    '''

    # Class data
    location : list = None
    rotation : list = None
    scale    : list = None

    json_data: dict = None
    # Class methods

    # Constructor
    def __init__(self, object: bpy.types.Object):

        self.json_data = { }

        # Location
        self.location = [ None, None, None ]
        self.location[0]             = round(object.location[0], 3)
        self.location[1]             = round(object.location[1], 3)
        self.location[2]             = round(object.location[2], 3)

        # Save the rotation mode
        temp                         = object.rotation_mode

        # Set the rotation mode to quaternion
        object.rotation_mode         = 'QUATERNION'

        # Rotation 
        self.rotation = [ None, None, None, None ]
        self.rotation[0]             = round(object.rotation_quaternion[0], 3)
        self.rotation[1]             = round(object.rotation_quaternion[1], 3)
        self.rotation[2]             = round(object.rotation_quaternion[2], 3)
        self.rotation[3]             = round(object.rotation_quaternion[3], 3)
        
        # Restore the rotation mode
        object.rotation_mode         = temp

        # Scale
        self.scale = [ None, None, None ]
        self.scale[0]                = round(object.scale[0], 3)
        self.scale[1]                = round(object.scale[1], 3)
        self.scale[2]                = round(object.scale[2], 3)

        # Set up the dictionary
        self.json_data["$schema"]    = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/transform-schema.json"
        self.json_data["location"]   = self.location.copy()
        self.json_data["quaternion"] = self.rotation.copy()
        self.json_data["scale"]      = self.scale.copy()

        return 

    # Returns class as JSON text
    def json(self):
        
        return json.dumps(self.json_data, indent=4)

class Rigidbody:
    
    '''
        - Rigidbody
    '''

    # Class data
    mass     : float = None
    active   : bool  = None

    json_data: dict  = None

    # Class methods

    @staticmethod
    def has_rigidbody(object: bpy.types.Object) -> bool:
        return True if isinstance(object.rigid_body, bpy.types.RigidBodyObject) else False

    def __init__(self, object: bpy.types.Object):
        
        # Exit if there is no rigidbody
        if Rigidbody.has_rigidbody(object) == False:
            return
        
        self.json_data = { }

        # Set class data
        self.mass   = object.rigid_body.mass 
        self.active = True if object.rigid_body.type == "ACTIVE" else False

        # Set up the dictionary
        self.json_data["$schema"]  = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/rigidbody-schema.json"
        self.json_data["active"]   = self.active
        self.json_data["friction"] = object.rigid_body.friction
        self.json_data["bounce"]   = object.rigid_body.restitution
 
        return 

    # Returns class as JSON text
    def json(self):
        if self.active:
            self.json_data["mass"]    = self.mass

        return json.dumps(self.json_data, indent=4)

    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Collider:
    '''
        - Collider
    '''

    # Class data
    dimensions : list = None
    shape      : str  = None
    convex_hull: str  = None

    json_data  : dict = None

    # Class methods
    @staticmethod
    def calculate_bounds (object) -> list:

        # [ MAX, MIN ]
        ret = [ [ 0, 0, 0 ], [ 0, 0, 0 ] ]

        temp_mat = object.matrix_world.copy()

        for i in object.data.vertices:
            
            # Min X
            if ret[0][0] < i.co[0]:
                ret[0][0] = i.co[0]
            
            # Max X
            if ret[1][0] > i.co[0]:
                ret[1][0] = i.co[0]
            
            # Min Y
            if ret[0][1] < i.co[1]:
                ret[0][1] = i.co[1]
            
            # Max Y
            if ret[1][1] > i.co[1]:
                ret[1][1] = i.co[1]
            
            # Min Z
            if ret[0][2] < i.co[2]:
                ret[0][2] = i.co[2]
            
            # Max Z
            if ret[1][2] > i.co[2]:
                ret[1][2] = i.co[2]
            
        object.matrix_world = temp_mat

        return ret

    # Constructor
    def __init__ (self, object: bpy.types.Object):
        
        # Check for a rigidbody
        if Rigidbody.has_rigidbody(object) == False:
            return

        # Set the shape
        self.shape = object.rigid_body.collision_shape

        # Set the dimensions

        z = self.calculate_bounds(object)
        self.max = z[0]
        self.min = z[1]

        # TODO

        # Set the JSON
        self.json_data               = { }
        self.json_data["$schema"]    = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/collider-schema.json"
        self.json_data["type"]       = self.shape
        self.json_data["max"]        = self.max
        self.json_data["min"]        = self.min

        # Write the convex hull
        if self.convex_hull is not None:
            self.json_data["convex hull path"] = self.convex_hull
        
        return
    
    def json(self):

        return json.dumps(self.json_data, indent=4)

    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Entity:
    '''
        - Entity
    '''

    name     : str       = None
    part     : Part      = None
    material : Material  = None
    transform: Transform = None
    rigidbody: Rigidbody = None
    collider : Collider  = None

    json_data: dict      = None

    path     : str       = None

    def __init__(self, object: bpy.types.Object):
        if isinstance(object.data, bpy.types.Mesh) == False:
            if object.type == 'EMPTY':
                self.name = object.name
                self.transform = Transform(object)

                self.json_data = { }

                self.json_data['$schema']   = 'https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/entity-schema.json'
                self.json_data['name']      = self.name

                if bool(self.transform.json_data):
                    self.json_data['transform'] = json.loads(self.transform.json())

                return
            else:
                return
        
        self.name      = object.name
        self.part      = Part(object)
        if len(object.material_slots) > 0:
            self.material  = materials.get(object.material_slots[0].material.name) if materials.get(object.material_slots[0].material.name) is not None else Material(object.material_slots[0].material)
        else:
            self.material  = None
        self.transform = Transform(object)
        self.rigidbody = Rigidbody(object)
        self.collider  = Collider(object)

        self.json_data = { }

        self.json_data['$schema']   = 'https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/entity-schema.json'
        self.json_data['name']      = self.name

        if bool(self.part.json_data):
            self.json_data['parts'] = []
            self.json_data['parts'].append(json.loads(self.part.json()))

        if bool(self.material):
            if bool(self.material.json_data):
                self.json_data['materials'] = []
                self.json_data['materials'].append(json.loads(self.material.json()))

        global export_context 

        self.json_data['shader'] = export_context['shader']

        print(f"EXPORT CONTEXT SHADER {export_context['shader']}")

        if bool(self.transform.json_data):
            self.json_data['transform'] = json.loads(self.transform.json(), parse_float=lambda x: round(float(x), 3))

        if bool(self.rigidbody.json_data):
            self.json_data['rigidbody'] = json.loads(self.rigidbody.json())

        if bool(self.collider.json_data):
            self.json_data['collider'] = json.loads(self.collider.json())

        if bool(object.parent):
            if isinstance(object.parent, bpy.types.Armature):
                self.rig = Rig(object.parent)
                self.json_data['rig'] = json.loads(self.rig.json())

        return
    
    def json (self):
        
        return (json.dumps(self.json_data, indent=4))

    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

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

class Skybox:

    '''
        - Skybox
    '''

    json_data: dict            = None
    image:     bpy.types.Image = None
    name :     str             = None

    def __init__ (self, world: bpy.types.World):
        
        # Check if there is a node to grab the equirectangular image from
        if bool(world.node_tree.nodes.find('Environment Texture')) == False:

            # If not, bail
            return

        self.name = world.name

        # Make a copy of the image
        self.json_data                = {}
        self.json_data['$schema']     = 'https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/skybox-schema.json'
        self.json_data['name']        = self.name
        self.json_data['environment'] = ""

        self.image = world.node_tree.nodes['Environment Texture'].image.copy()

        return

    def save_image (self, path: str):
        
        if self.image is not None:

            # Preserve the image rendering type
            tmp = bpy.context.scene.render.image_settings.file_format

            # Set the image type to Radiance HDR
            bpy.context.scene.render.image_settings.file_format = 'HDR'
            
            # Save the image to the specified path
            self.image.save_render(path)

            # Restore the image type
            bpy.context.scene.render.image_settings.file_format = tmp

            self.json_data['environment'] = path
        
        return
    
    def json (self):

        return (json.dumps(self.json_data, indent=4))

    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:

            # Try to write the file
            try:
                f.write(self.json())

            # Exceptions
            except FileExistsError:

                pass

        return

    def __del__ (self):

        # Check if there is an image to remove
        if self.image is not None:

            # Remove the copy made in the constructor
            bpy.data.images.remove(self.image)
        
        return

class Scene:

    '''
        - Scene
    '''
    
    name         : str           = None
    entities     : list          = None
    cameras      : list          = None
    lights       : list          = None
    light_probes : list          = None

    skybox       : Skybox        = None

    json_data    : dict          = None

    def __init__(self, scene: bpy.types.Scene):

        # Check for the right type
        if isinstance(scene, bpy.types.Scene) == False:
            # TODO: Throw an exception?
            return

        # Scene name
        self.name         = scene.name

        # Scene lists
        self.entities     = []
        self.cameras      = []
        self.lights       = []
        self.light_probes = []
        self.json_data    = { }

        # Scene JSON
        self.json_data["$schema"]      = "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/scene-schema.json"
        self.json_data["name"]         = scene.name
        self.json_data["entities"]     = []
        self.json_data["cameras"]      = []
        self.json_data["lights"]       = []
        self.json_data["light probes"] = []
        self.json_data["skybox"]       = {}
        
        # Iterate over each object in the scene
        for object in scene.objects:

            # Construct a light 
            if object.type == 'LIGHT':
                self.lights.append(Light(object))

            # Construct a camera
            elif object.type == 'CAMERA':
                self.cameras.append(Camera(object))

            # Construct an entity
            elif object.type == 'MESH' or object.type == 'EMPTY':
                self.entities.append(Entity(object))

            # Construct a light probe 
            elif object.type == 'LIGHT_PROBE':
                self.light_probes.append(LightProbe(object))            

            # Default case
            else:
                print("[gport] [Export] [Scene] Unrecognized object in scene \"" + scene.name + "\"")
            
        # If the scene has a world
        if isinstance(scene.world, bpy.types.World):
            
            # If the world has a 'World Output' node in the node_tree
            if scene.world.node_tree.nodes.find('World Output') > -1:

                # If there is a link to another node
                if len(scene.world.node_tree.nodes['World Output'].inputs[0].links) == 1:

                    # If there is a 'Background' node
                    if scene.world.node_tree.nodes['World Output'].inputs[0].links[0].from_node.type == 'BACKGROUND':
                        
                        # Construct the skybox
                        self.skybox = Skybox(scene.world)
            
        return

    def json(self):


        if bool(self.json_data["entities"])     == False :
            self.json_data.pop("entities")

        if bool(self.json_data["cameras"])      == False :
            self.json_data.pop("cameras")

        if bool(self.json_data["lights"])       == False :
            self.json_data.pop("lights")

        if bool(self.json_data["light probes"]) == False :
            self.json_data.pop("light probes")

        if bool(self.json_data["skybox"])       == False :
            self.json_data.pop("skybox")

        return json.dumps(self.json_data,indent=4)

    def write_to_directory(self, directory: str):
        
        """
            Writes a scene to a directory, with entities, materials, parts, colliders, and skyboxes.
        """

        # Make scene directories

        # This is where the scene is exported
        try   : os.mkdir(directory)
        except: pass

        # This is where audio is exported
        try   : os.mkdir(directory + "/audio/")
        except: pass

        # This is where convex hulls are exported
        try   : os.mkdir(directory + "/colliders/")
        except: pass
        
        # This is where entities are exported
        try   : os.mkdir(directory + "/entities/")
        except: pass
        
        # This is where materials are exported
        try   : os.mkdir(directory + "/materials/")
        except: pass
        
        # This is where 3D models are exported
        try   : os.mkdir(directory + "/parts/")
        except: pass

        # This is where the scene is exported
        try   : os.mkdir(directory + "/scenes/")
        except: pass
        
        # This is where the skybox is exported
        try   : os.mkdir(directory + "/skyboxes/")
        except: pass

        # This is where material textures are exported
        # NOTE: Material textures are written to "textures/[material name]/". 
        try   : os.mkdir(directory + "/textures/")
        except: pass
        
        # Write entities
        if bool(self.entities) == True:

            # Make an entity array in the json object
            self.json_data["entities"] = []

            # Save each entity
            for entity in self.entities:

                # Write the entity and all its data
                e_json = entity.write_to_directory(directory)

                # Write the entity path into the entities array
                self.json_data["entities"].append(json.loads(e_json))

                # Destruct the entity
                del entity

        # Write cameras
        if bool(self.cameras) == True:

            # Make a camera array in the json object
            self.json_data["cameras"] = []

            # Save each camera
            for camera in self.cameras:

                # Write the camera json object into the cameras array
                self.json_data["cameras"].append(json.loads(camera.json()))

                # Destruct the camera
                del camera

        # Write lights
        if bool(self.lights) == True:

            # Make a light array in the json object
            self.json_data["lights"] = []

            # Save each light
            for light in self.lights:

                # Write the light json object into the lights array
                self.json_data["lights"].append(json.loads(light.json()))

                # Destruct the light
                del light


        # Write light probes
        if bool(self.light_probes) == False:

            # Make a light probe array in the json object
            self.json_data["light probes"] = []

            # Save each light probe
            for light_probe in self.light_probes:

                #self.json_data["light probes"].append(light_probe.json())

                #del light_probe
                pass

        # Write the skybox
        if bool(self.skybox) == True:
            
            # Save the skybox image
            self.skybox.save_image(directory + "/skyboxes/" + self.skybox.name + ".hdr")

            # Save the skybox json
            self.skybox.write_to_file(directory + "/skyboxes/" + self.skybox.name + ".json")

            # Make a reference to the skybox json file in the json object
            self.json_data["skybox"]       = directory + "/skyboxes/" + self.skybox.name + ".json"


        # The path to the scene
        path = directory + "/scenes/" + self.name + ".json"

        # Write the JSON data to the path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Bone:
    '''
        - Bone
    '''

    name        : str  = None

    bone_matrix : list = None
    bone_head   : list = None
    bone_tail   : list = None
    json_data   : dict = None
    
    children    : list = None

    # Constructor
    def __init__(self, bone: bpy.types.Bone, bone_names_and_indexes):
        
        self.name        = bone.name
        self.bone_matrix = bone.matrix
        self.bone_head   = [ bone.head[0], bone.head[1], bone.head[2] ]
        self.bone_tail   = [ bone.tail[0], bone.tail[1], bone.tail[2] ]

        self.json_data = {}
        self.json_data['name']  = self.name
        self.json_data['head']  = self.bone_head
        self.json_data['tail']  = self.bone_tail
        self.json_data['index'] = bone_names_and_indexes[self.name]


        if bool(bone.children):
            self.children = [  ]
            self.json_data['children'] = [  ]
            for b_i, b in enumerate(bone.children):
                self.children.append(Bone(b, bone_names_and_indexes))
                self.json_data['children'].append(json.loads(self.children[b_i].json()))

        return

    # Returns file JSON
    def json(self):

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Pose:

    '''
        - Pose
    '''
    
    name          : str   = None

    json_data     : dict  = None
    delta         : float = None

    # Constructor
    def __init__(self, object: bpy.types.NlaStrip):

        if isinstance(object, bpy.types.NlaStrip) == False:
            return 

        self.name  = object.name

        # Keyframes must be converted into keytimes. Great application of dimensional analysis
        # 
        # second = frames / ( frames / second )
        #        = frames * ( second / frames ) 
        #        = second * ( frames / frames )

        self.delta = object.frame_start / float(bpy.context.scene.render.fps)


        return

    # Returns file JSON
    def json(self):
        self.json_data          = {}
        self.json_data['name']  = self.name
        self.json_data['delta'] = self.delta
        
        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Action:

    '''
        - Action
    '''

    name          : str  = None

    json_data     : dict = None
    poses         : list = None
    pose_sequence : list = None

    @staticmethod
    def strip_sort(e):
        return e.frame_start


    # Constructor
    def __init__(self, object: bpy.types.NlaTrack):

        if isinstance(object, bpy.types.NlaTrack) == False:
            return 


        self.name = object.name

        self.poses = []
        self.pose_sequence = []

        for p_i in sorted(object.strips, key=self.strip_sort):
            self.pose_sequence.append(Pose(p_i))
        
        self.json_data = {}
        self.json_data['poses']         = []
        self.json_data['pose sequence'] = []

        return

    # Returns file JSON
    def json(self):

        
        self.json_data['name']          = self.name


        for p_i in self.pose_sequence:
            self.json_data['pose sequence'].append(json.loads(p_i.json()))

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return

class Rig:

    '''
        - Rig
    '''

    name       : str            = None

    json_data  : dict           = None
    root_bone  : bpy.types.Bone = None
    bones_json : dict           = None
    bone       : Bone           = None
    actions    : list           = None

    # Constructor
    def __init__(self, object: bpy.types.Object):

        # Type check
        if isinstance(object.data, bpy.types.Armature) == False:
            return

        context_action = object.animation_data.action

        # Construct a dictionary
        self.json_data = { }

        self.name = object.name

        # Grab a random bone
        b = object.pose.bones[0]

        # Find the root bone 
        while b.parent != None:
            b = b.parent[0]

        # Make sure there is a valid part first
        bone_names_and_indexes = parts[object.children[0].name].get_bone_names_and_indexes(object.children[0])

        self.actions = []

        if len(object.animation_data.nla_tracks) > 0:
            for action in object.animation_data.nla_tracks:
                self.actions.append(Action(action))

        for action in self.actions:
            for pose in action.pose_sequence:

                z = { }

                object.animation_data.action = bpy.data.actions[object.animation_data.nla_tracks[action.name].strips[pose.name].action.name]

                bpy.context.scene.frame_set( int(pose.delta * bpy.context.scene.render.fps))
                
                z['name'] = pose.name
                z['bones'] = json.loads(Bone(object.pose.bones[0], bone_names_and_indexes).json())
                action.json_data['poses'].append(json.loads(json.dumps(z)))

                z = None


        self.bone = Bone(b, bone_names_and_indexes)

        self.json_data = { }
        self.json_data['$schema']          = 'https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/rig-schema.json'
        self.json_data['name']             = self.name
        self.json_data['bone count']       = len(object.data.bones)
        self.json_data['part name']        = object.children[0].name
        self.json_data['actions']          = []

        if len(self.actions) > 0:
            for a in self.actions:
                self.json_data['actions'].append(json.loads(a.json()))

        self.json_data['bones']            = json.loads(self.bone.json())

        object.animation_data.action = context_action

        return

    # Returns file JSON
    def json(self):

        return json.dumps(self.json_data, indent=4)

    # Writes JSON to a specified file
    def write_to_file(self, path: str):
        
        # Write the JSON data to the specified path
        with open(path, "w+") as f:
            try:
                f.write(self.json())
            except FileExistsError:
                pass

        return