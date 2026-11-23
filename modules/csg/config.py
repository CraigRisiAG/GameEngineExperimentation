# This file provides configuration for the CSG module within the Godot engine.
# It includes functions to determine if the module can be built for a specific
# platform and to configure the build environment accordingly.
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "CSGBox",
        "CSGCombiner",
        "CSGCylinder",
        "CSGMesh",
        "CSGPolygon",
        "CSGPrimitive",
        "CSGShape",
        "CSGSphere",
        "CSGTorus",
    ]

def get_doc_path():
    return "doc_classes"
