# This file provides the build configuration for the Bullet module in the Godot engine.
# It defines functions to check if the module can be built for a given platform,
# configure the build environment, and provide documentation paths and classes.
def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "BulletPhysicsDirectBodyState",
        "BulletPhysicsServer",
    ]

def get_doc_path():
    return "doc_classes"
