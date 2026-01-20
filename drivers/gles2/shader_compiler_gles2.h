

/**
 * @class ShaderCompilerGLES2
 * @brief Compiler for GLES2 shaders that converts shader code into platform-specific generated code.
 * 
 * This class handles the compilation of shader language code into GLES2-compatible vertex, fragment,
 * and light shader code. It manages identifier actions, render modes, uniforms, and shader-specific
 * time and usage tracking.
 * 
 * @struct IdentifierActions
 * @brief Container for shader identifier configuration and tracking data.
 * @details Holds mappings for render modes, usage flags, write flags, and uniform definitions
 *          that are applied during shader compilation.
 * 
 * @struct GeneratedCode
 * @brief Output structure containing the compiled shader code and metadata.
 * @details Contains the compiled vertex, fragment, and light shader code along with custom defines,
 *          uniforms, texture information, and time usage flags.
 * 
 * @struct DefaultIdentifierActions
 * @brief Internal structure mapping identifier strings to their GLSL replacements and defines.
 * @details Provides rename mappings, render mode defines, and usage defines for shader compilation.
 * 
 * @method Error compile(VS::ShaderMode p_mode, const String &p_code, IdentifierActions *p_actions, const String &p_path, GeneratedCode &r_gen_code)
 * @brief Compiles shader code for the specified shader mode.
 * @param p_mode The shader mode (vertex, fragment, etc.)
 * @param p_code The source shader code to compile
 * @param p_actions Pointer to identifier actions configuration
 * @param p_path The file path of the shader (for debugging)
 * @param r_gen_code Output structure containing the generated shader code
 * @return Error code indicating success or failure of compilation
 * 
 * @constructor ShaderCompilerGLES2()
 * @brief Initializes a new shader compiler instance.
 */
#ifndef SHADERCOMPILERGLES2_H
#define SHADERCOMPILERGLES2_H

#include "core/pair.h"
#include "core/string_builder.h"
#include "servers/visual/shader_language.h"
#include "servers/visual/shader_types.h"
#include "servers/visual_server.h"

class ShaderCompilerGLES2 {
public:
	struct IdentifierActions {

		Map<StringName, Pair<int *, int>> render_mode_values;
		Map<StringName, bool *> render_mode_flags;
		Map<StringName, bool *> usage_flag_pointers;
		Map<StringName, bool *> write_flag_pointers;

		Map<StringName, ShaderLanguage::ShaderNode::Uniform> *uniforms;
	};

	struct GeneratedCode {

		Vector<CharString> custom_defines;
		Vector<StringName> uniforms;
		Vector<StringName> texture_uniforms;
		Vector<ShaderLanguage::DataType> texture_types;
		Vector<ShaderLanguage::ShaderNode::Uniform::Hint> texture_hints;

		String vertex_global;
		String vertex;
		String fragment_global;
		String fragment;
		String light;

		bool uses_fragment_time;
		bool uses_vertex_time;
	};

private:
	ShaderLanguage parser;

	struct DefaultIdentifierActions {

		Map<StringName, String> renames;
		Map<StringName, String> render_mode_defines;
		Map<StringName, String> usage_defines;
	};

	void _dump_function_deps(ShaderLanguage::ShaderNode *p_node, const StringName &p_for_func, const Map<StringName, String> &p_func_code, StringBuilder &r_to_add, Set<StringName> &r_added);
	String _dump_node_code(ShaderLanguage::Node *p_node, int p_level, GeneratedCode &r_gen_code, IdentifierActions &p_actions, const DefaultIdentifierActions &p_default_actions, bool p_assigning);

	StringName current_func_name;
	StringName vertex_name;
	StringName fragment_name;
	StringName light_name;
	StringName time_name;

	Set<StringName> used_name_defines;
	Set<StringName> used_flag_pointers;
	Set<StringName> used_rmode_defines;
	Set<StringName> internal_functions;

	DefaultIdentifierActions actions[VS::SHADER_MAX];

public:
	Error compile(VS::ShaderMode p_mode, const String &p_code, IdentifierActions *p_actions, const String &p_path, GeneratedCode &r_gen_code);

	ShaderCompilerGLES2();
};

#endif // SHADERCOMPILERGLES2_H
