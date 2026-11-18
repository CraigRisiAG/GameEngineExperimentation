# This file provides the build configuration for the ARKit module, specifying
# the platforms it can be built for and any necessary environment setup.
def can_build(env, platform):
    return platform == 'iphone'

def configure(env):
    pass
