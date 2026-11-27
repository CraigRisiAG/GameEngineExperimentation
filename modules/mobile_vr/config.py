# This file provides the configuration for the MobileVR module in the Godot engine.
# It defines functions for determining build support, configuring the environment,
# and specifying documentation classes and paths.
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "MobileVRInterface",
    ]

def get_doc_path():
    return "doc_classes"
