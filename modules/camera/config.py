# This file provides configuration for the camera module within the Godot engine.
# It includes functions to determine if the module can be built for a specific
# platform and to configure the build environment accordingly.
def can_build(env, platform):
     return platform == 'iphone' or platform == 'osx' or platform == 'windows'

def configure(env):
    pass
