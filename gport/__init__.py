# 
# GPort is an Import-Export Addon for Blender, and a tool for the G10 Engine. 
#
# To use GPort, press Ctrl+Alt+U, navigate to Add-ons, and search for "GPort". 
# Create a project in the Preferences panel by specifying a name and a directory. 
# Set up your Blender scene. When finished, navigate to File > Export > G10 Scene (.json)
# Configure your export, and wait for your machine to finish.
#
# WARNING: This operation can take a lot time, and a lot of GPU Power and CPU Power. 
# In other words, your machine may become unusable during the export operation. 
# Don't panic if your Blender window freezes. Just wait patiently. It will finish. 
#
# Ideally you have a second machine you can run this operation on, but if that is not the case, 
#

bl_info = {
    "name"        : "gport",
    "description" : "g10 / Blender reflection",
    "author"      : "Jacob Smith",
    "version"     : (0, 1),
    "blender"     : (3, 6, 0),
    "location"    : "File > Import-Export",
    "warning"     : "This software has not yet been rigorously tested, and may not meet commercial software standards",
    "doc_url"     : "https://github.com/Jacob-C-Smith/gport/",
    "category"    : "Import-Export",
}

import bpy, bmesh
import math, json, sys, time, os, getpass, importlib

from struct              import pack
from dataclasses         import dataclass
from timeit              import default_timer              as timer
from bpy_extras.io_utils import ExportHelper, ImportHelper
from bpy.types           import Operator, AddonPreferences
from bpy.props           import (
    StringProperty,
    BoolProperty,
    BoolVectorProperty,
    EnumProperty,
    IntProperty,
    FloatProperty,
    CollectionProperty
)

from .gxport import gxport
from .g10_blender import ( 
    Light,
    Camera,
    Geometry,
    Part,
    Texture,
    Material,
    LightProbe,
    Transform,
    Rigidbody,
    Collider,
    Entity,
    Skybox,
    Scene,
    Bone,
    Pose,
    Action,
    Rig
)

materials:  dict = {}
entities:   dict = {}
parts:      dict = {}
g10_source: dict = os.environ["G10_SOURCE_PATH"]

class GPropertyCollection          ( bpy.types.PropertyGroup ):
    """
    A class used to store project path data

    name - A bpy.types.StringProperty storing the name of the project
    path - A bpy.types.StringProperty storing the project's path on the filesystem.
    
    """

    name: StringProperty()
    path: StringProperty(subtype="DIR_PATH")

class GPort_Preferences            ( AddonPreferences ):
    """
    A class for addon preferences

    prop_collection - A bpy.props.CollectionProperty for storing the project list
    
    """

    # This is for Blender
    bl_idname = __name__

    # This is a collection of project names and paths
    prop_collection : bpy.props.CollectionProperty(type=GPropertyCollection)

    # This gets called when the class is registered. 
    @classmethod
    def register ( self ):

        if len(bpy.context.preferences.addons['gport'].preferences.prop_collection) < 1:
            bpy.context.preferences.addons['gport'].preferences.prop_collection.add()

        return

    # This gets called to draw the addon preferences panel
    def draw(self, context):

        # The addon preferences panel
        layout = self.layout

        # Make a box
        box    = layout.box()

        # Project path
        box.label(text="projects", icon='FILEBROWSER')

        # Divide the box into 2 columns. One for name and one for path
        split  = box.split(factor=0.35)
        name_c = split.column()
        path_c = split.column()
        
        #The name column
        r = name_c.row(align=True)
        r.separator()
        r.label(text="name")

        # The path column
        r = path_c.row(align=True)
        r.separator()
        r.label(text="path")

        # Add a button to remove each entry
        i = 0

        for prop in self.prop_collection:

            r = name_c.row()
            r.prop(prop, "name", text='')

            r = path_c.row()
            sr = r.row()
            sr.prop(prop, "path", text='')
            r.operator("gport.remove_project_path", text='',icon='X', emboss=False).index = i

            i = i + 1
        
        # Add a buton to add a new project
        r = box.row()
        r.alignment = 'RIGHT'
        r.operator("gport.add_project_path", text='',icon='ADD', emboss=False)

class GPORT_OT_add_project_path    ( Operator ):
    """
    An operation class for adding a new project in the addon preferences panel
    """
    
    bl_idname = "gport.add_project_path"
    bl_label = "Add project"

    def execute(self, context):
        bpy.context.preferences.addons['gport'].preferences.prop_collection.add()
        return {'FINISHED'}

class GPORT_OT_remove_project_path ( Operator ):

    """
    An operation class for adding a new project in the addon preferences

    index : A bpy.types.IntProperty initialized to the project position in the addon preferences panel
    """
    

    bl_idname = "gport.remove_project_path"
    bl_label = "Remove project"

    index : bpy.props.IntProperty()

    def execute(self, context):
        bpy.context.preferences.addons['gport'].preferences.prop_collection.remove(self.index)
        return {'FINISHED'}

# TODO: Add gimport
c = (

    # This class holds project path information
    GPropertyCollection,

    # These classes are operators for addon preferences panel
    GPORT_OT_add_project_path,
    GPORT_OT_remove_project_path,

    # This class is the adoon preference panel
    GPort_Preferences,
    
    # This class is the export operator ( File > Export > G10 Scene (.json) )
    gxport,

    # This class is the import operator ( File > Import > G10 Scene (.json) )
    #gximport,
)

# This function is called to add export options to File > Export
def menu_func_export(self, context):
    self.layout.operator(gxport.bl_idname, text="G10 Scene (.json)")

def menu_func_import(self, context):
    self.layout.operator(gimport.bl_idname, text="Import G10 Scene (.json)")

def register():
    
    # Iterate over each class
    for cl in c:

        # Register the iterated class
        bpy.utils.register_class(cl)

    # Add an export option under File > Export > 
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)

    # Add an import option under File > Import > 
    #bpy.types.TOPBAR_MT_file_import.append(menu_func_import)

def unregister():

    for cl in c:
        bpy.utils.unregister_class(cl)

    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
    #bpy.types.TOPBAR_MT_file_export.remove(menu_func_import)

if __name__ == "__main__":
    register()