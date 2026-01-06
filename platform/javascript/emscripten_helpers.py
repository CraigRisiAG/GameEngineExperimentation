"""
Parse the Emscripten configuration file and extract relevant settings.
Reads the Emscripten configuration file from the EM_CONFIG environment variable
or from the default location ~/.emscripten. The configuration file is executed
as Python code to extract settings.
Returns:
    dict: A normalized dictionary containing:
        - 'EMCC_ROOT': The Emscripten root directory
        - 'NODE_JS': The path to the Node.js executable
        - 'CLOSURE_BIN': The path to the Google Closure Compiler binary
Raises:
    RuntimeError: If the configuration file does not exist or is invalid Python code.
"""
"""
Generate a command string to run the Google Closure Compiler on JavaScript files.
Constructs a command to invoke the Closure Compiler with advanced optimizations
on the provided source files, applying any specified extern declarations.
Args:
    target: The target file(s) where compiled output will be written.
    source: List of source JavaScript files to compile.
    env: SCons environment containing 'JSEXTERNS' list of extern files and 'NODE_JS' path.
    for_signature: SCons signature parameter (unused).
Returns:
    str: The complete Closure Compiler command as a space-joined string.
"""
"""
Create an engine JavaScript file, either compiled or concatenated.
Decides whether to compile JavaScript files using the Closure Compiler or
simply concatenate them based on the 'use_closure_compiler' environment flag.
Args:
    env: SCons environment with 'use_closure_compiler' flag and build methods.
    target: The target file to create.
    source: List of source JavaScript files to process.
    externs: List of extern declarations to pass to the Closure Compiler.
Returns:
    The result of either BuildJS() or Textfile() depending on compilation settings.
"""
import os
def parse_config():
    em_config_file = os.getenv('EM_CONFIG') or os.path.expanduser('~/.emscripten')
    if not os.path.exists(em_config_file):
        raise RuntimeError("Emscripten configuration file '%s' does not exist" % em_config_file)

    normalized = {}
    em_config = {}
    with open(em_config_file) as f:
        try:
            # Emscripten configuration file is a Python file with simple assignments.
            exec(f.read(), em_config)
        except StandardError as e:
            raise RuntimeError("Emscripten configuration file '%s' is invalid:\n%s" % (em_config_file, e))
    normalized['EMCC_ROOT'] = em_config.get('EMSCRIPTEN_ROOT')
    normalized['NODE_JS'] = em_config.get('NODE_JS')
    normalized['CLOSURE_BIN'] = os.path.join(normalized['EMCC_ROOT'], 'node_modules', '.bin', 'google-closure-compiler')
    return normalized


def run_closure_compiler(target, source, env, for_signature):
    cfg = parse_config()
    cmd = [cfg['NODE_JS'], cfg['CLOSURE_BIN']]
    cmd.extend(['--compilation_level', 'ADVANCED_OPTIMIZATIONS'])
    for f in env['JSEXTERNS']:
        cmd.extend(['--externs', f.get_abspath()])
    for f in source:
        cmd.extend(['--js', f.get_abspath()])
    cmd.extend(['--js_output_file', target[0].get_abspath()])
    return ' '.join(cmd)


def create_engine_file(env, target, source, externs):
    if env['use_closure_compiler']:
        return env.BuildJS(target, source, JSEXTERNS=externs)
    return env.Textfile(target, [env.File(s) for s in source])
