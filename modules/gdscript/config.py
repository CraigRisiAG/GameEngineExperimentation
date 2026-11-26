# This file provides configuration functions for the GDScript module within the
# Godot engine. It defines functions to check if the module can be built for a
# given platform, configure the build environment, and provide documentation
# paths and classes related to GDScript.    
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "@GDScript",
        "GDScript",
        "GDScriptFunctionState",
        "GDScriptNativeClass",
    ]

def get_doc_path():
    return "doc_classes"
