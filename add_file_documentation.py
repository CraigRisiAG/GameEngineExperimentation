#!/usr/bin/env python3
"""
Batch documentation generator for GameEngineExperimentation source files.

This script adds Doxygen @file documentation to all source files (*.h, *.cpp)
that lack documentation headers. It intelligently infers documentation from:
- File name and location (core/, scene/, servers/, etc.)
- File content (class/struct declarations)
- Common patterns in the codebase

Usage:
    python add_file_documentation.py [--dry-run] [--verbose]

Options:
    --dry-run     Show what would be changed without modifying files
    --verbose     Print detailed information about each file
"""

import os
import re
import sys
from pathlib import Path
from typing import Tuple, Optional

# Documentation templates by directory and file purpose
DOCS_BY_CATEGORY = {
    "core": {
        "object": "Core object base class for the engine's reflection system.",
        "variant": "Dynamic typing system supporting multiple data types.",
        "class_db": "Reflection system database for class introspection and binding.",
        "resource": "Base class for serializable engine resources.",
        "vector": "Template container for dynamic arrays with pre-allocation support.",
        "array": "Variant-based dynamic array container.",
        "dictionary": "Hash map container for key-value pairs.",
        "string": "Unicode string handling and manipulation.",
        "hash_map": "Template-based hash table implementation.",
        "map": "Template-based ordered map (red-black tree).",
        "list": "Template-based doubly-linked list.",
        "set": "Template-based hash set.",
        "engine": "Central engine singleton managing core subsystems.",
        "callable": "Type-safe function/method wrapper for callbacks.",
        "method_bind": "Method binding for reflection and script exposure.",
        "reference": "Reference-counted memory management for resources.",
        "rid": "Resource ID handle for server objects.",
        "error": "Error handling and assertion macros.",
        "project_settings": "Runtime configuration and project settings management.",
        "input_map": "Input event mapping and action bindings.",
        "print_string": "Debug logging and console output utilities.",
    },
    "scene": {
        "node": "Scene tree node - base class for all scene objects.",
        "scene_tree": "Main scene management and processing system.",
        "viewport": "Rendering target and input handling viewport.",
        "canvas_layer": "2D rendering layer for canvas-based rendering.",
        "timer": "Timeout and interval timer node.",
        "resource_preloader": "Pre-loads resources at scene startup.",
        "http_request": "Asynchronous HTTP client node.",
        "instance_placeholder": "Placeholder for scene instancing.",
    },
    "servers": {
        "visual_server": "Rendering command queue and graphics API abstraction.",
        "physics": "Physics simulation engine interface (3D/2D).",
        "audio_server": "Audio playback and processing system.",
        "camera_server": "Camera feed management for video input.",
        "navigation": "Pathfinding and navigation mesh system.",
    },
    "editor": {
        "dependency_editor": "File dependency management and validation dialogs.",
        "editor_node": "Main editor window and project management.",
        "editor_file_dialog": "File selection dialog for editor operations.",
    },
    "tests": {
        "test": "Unit and integration tests for verification.",
        "benchmark": "Performance benchmarking and profiling tests.",
    }
}

COPYRIGHT_PATTERN = re.compile(r'^/\*+.*?GODOT ENGINE.*?\*+/$', re.MULTILINE | re.DOTALL)
DOXYGEN_PATTERN = re.compile(r'^\s*/\*\*\s*@file', re.MULTILINE)
IFNDEF_PATTERN = re.compile(r'^#ifndef\s+\w+', re.MULTILINE)


def get_file_category(file_path: Path) -> str:
    """Determine file category from path."""
    parts = file_path.parts
    if 'core' in parts:
        return 'core'
    elif 'scene' in parts:
        return 'scene'
    elif 'servers' in parts:
        return 'servers'
    elif 'editor' in parts:
        return 'editor'
    elif 'drivers' in parts:
        return 'drivers'
    elif 'tests' in parts:
        return 'tests'
    return 'other'


def infer_documentation(file_path: Path, content: str) -> Optional[str]:
    """Infer documentation from file content and path."""
    file_stem = file_path.stem.lower()
    category = get_file_category(file_path)
    
    # Try to find matching documentation
    if category in DOCS_BY_CATEGORY:
        for key, doc in DOCS_BY_CATEGORY[category].items():
            if key in file_stem:
                return doc
    
    # Try to infer from content
    class_match = re.search(r'class\s+(\w+)\s*(?::|{)', content)
    struct_match = re.search(r'struct\s+(\w+)\s*(?::|{)', content)
    
    if class_match:
        class_name = class_match.group(1)
        return f"Implementation of {class_name} class."
    elif struct_match:
        struct_name = struct_match.group(1)
        return f"Definition of {struct_name} data structure."
    
    # Default documentation based on file type
    if file_path.suffix == '.h':
        return f"Header file for {file_stem} functionality."
    else:
        return f"Implementation of {file_stem} functionality."


def add_documentation(file_path: Path, verbose: bool = False) -> Tuple[bool, str]:
    """
    Add documentation to a file if it doesn't already have it.
    
    Returns:
        Tuple of (was_modified, message)
    """
    try:
        content = file_path.read_text(encoding='utf-8', errors='replace')
        
        # Check if already documented
        if DOXYGEN_PATTERN.search(content):
            if verbose:
                return False, f"Already documented: {file_path.name}"
            return False, None
        
        # Find copyright header end
        copyright_match = COPYRIGHT_PATTERN.search(content)
        if not copyright_match:
            if verbose:
                return False, f"No copyright header found: {file_path.name}"
            return False, None
        
        # Find insertion point (after copyright, before #ifndef)
        insertion_pos = copyright_match.end()
        
        # Infer documentation
        doc = infer_documentation(file_path, content)
        if not doc:
            if verbose:
                return False, f"Could not infer documentation: {file_path.name}"
            return False, None
        
        # Create documentation block
        doc_block = f"\n\n/**\n * @file {file_path.name}\n * @brief {doc}\n */\n"
        
        # Insert documentation
        new_content = content[:insertion_pos] + doc_block + content[insertion_pos:]
        file_path.write_text(new_content, encoding='utf-8')
        
        return True, f"Documented: {file_path.name}"
        
    except Exception as e:
        return False, f"Error processing {file_path.name}: {str(e)}"


def main():
    """Main entry point."""
    dry_run = '--dry-run' in sys.argv
    verbose = '--verbose' in sys.argv
    
    root = Path('.')
    
    # Find all source files
    source_dirs = ['core', 'scene', 'servers', 'editor', 'drivers', 'tests']
    files_to_process = []
    
    for src_dir in source_dirs:
        path = root / src_dir
        if path.exists():
            files_to_process.extend(path.rglob('*.h'))
            files_to_process.extend(path.rglob('*.cpp'))
    
    # Filter out thirdparty
    files_to_process = [f for f in files_to_process if 'thirdparty' not in str(f)]
    
    print(f"Processing {len(files_to_process)} source files...")
    if dry_run:
        print("(DRY RUN - no files will be modified)\n")
    
    modified_count = 0
    error_count = 0
    
    for file_path in sorted(files_to_process):
        was_modified, message = add_documentation(file_path, verbose)
        
        if was_modified:
            modified_count += 1
            if verbose:
                print(f"  ✓ {message}")
            elif modified_count % 50 == 0:
                print(f"  Documented {modified_count} files...")
        elif message and verbose:
            print(f"  - {message}")
        elif message is None and not verbose:
            pass  # Already documented, silent
        else:
            error_count += 1
            if verbose:
                print(f"  ✗ {message}")
    
    print(f"\n{'='*60}")
    print(f"Summary:")
    print(f"  Files processed: {len(files_to_process)}")
    print(f"  New docs added: {modified_count}")
    print(f"  Errors: {error_count}")
    print(f"{'='*60}")
    
    if dry_run:
        print("\nTo apply changes, run without --dry-run flag")


if __name__ == '__main__':
    main()
