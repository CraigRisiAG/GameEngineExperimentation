# This file provides the build configuration for the ENet module within the Godot engine.
# It includes functions to check if the module can be built for a given environment and
# platform, and to configure the build environment accordingly.
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "NetworkedMultiplayerENet",
    ]

def get_doc_path():
    return "doc_classes"
