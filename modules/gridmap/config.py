# This file provides the configuration for the gridmap module within the Godot
# engine. It defines functions to determine if the module can be built for a
# given environment and platform, and to configure the build environment
# accordingly.
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "GridMap",
    ]

def get_doc_path():
    return "doc_classes"
