
import os
"""
Generate debug symbols for macOS executables and strip the binary.
This function creates a dSYM (debug symbols) file for a macOS target binary
and then strips the executable. It supports both MacPorts LLVM and the system
dsymutil tool.
Args:
    target: List containing the target executable path (target[0] is used).
    source: List of source files (unused in this function).
    env: Build environment dictionary containing:
        - macports_clang (str): MacPorts LLVM version or 'no' to use system dsymutil.
Returns:
    None
Side Effects:
    - Creates a .dSYM file alongside the target executable.
    - Strips the target executable of debug symbols and other content.
Note:
    Requires either MacPorts LLVM tools or macOS dsymutil to be installed.
"""
from platform_methods import subprocess_main


def make_debug_osx(target, source, env):
    if (env["macports_clang"] != 'no'):
        mpprefix = os.environ.get("MACPORTS_PREFIX", "/opt/local")
        mpclangver = env["macports_clang"]
        os.system(mpprefix + '/libexec/llvm-' + mpclangver + '/bin/llvm-dsymutil {0} -o {0}.dSYM'.format(target[0]))
    else:
        os.system('dsymutil {0} -o {0}.dSYM'.format(target[0]))
    os.system('strip -u -r {0}'.format(target[0]))


if __name__ == '__main__':
    subprocess_main(globals())
