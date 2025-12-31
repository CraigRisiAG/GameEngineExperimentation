cmake_minimum_required(VERSION 3.16)

project(GameEngine CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform detection
if(MSVC)
    add_compile_definitions(WINDOWS_ENABLED MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++17")
elseif(APPLE)
    add_compile_definitions(OSX_ENABLED CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")
else()
    add_compile_definitions(LINUX_ENABLED GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")
endif()

# Find required packages
find_package(Threads REQUIRED)

# Core library
add_library(engine_core OBJECT
    core/os/os.h
    core/message_queue.h
)
set_target_properties(engine_core PROPERTIES LINKER_LANGUAGE CXX)

# Scene library
add_library(engine_scene OBJECT
    scene/main/node.h
    scene/main/scene_tree.h
)
set_target_properties(engine_scene PROPERTIES LINKER_LANGUAGE CXX)

# Servers library
add_library(engine_servers OBJECT
    servers/audio_server.h
    servers/physics_2d_server.h
    servers/physics_server.h
    servers/visual_server.h
)
set_target_properties(engine_servers PROPERTIES LINKER_LANGUAGE CXX)

# Main performance module
add_library(performance_module OBJECT
    Main/performance.cpp
    Main/performance.h
)
target_link_libraries(performance_module PUBLIC Threads::Threads)

# Main executable (if needed)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Main/main.cpp")
    add_executable(game_engine
        Main/main.cpp
        $<TARGET_OBJECTS:engine_core>
        $<TARGET_OBJECTS:engine_scene>
        $<TARGET_OBJECTS:engine_servers>
        $<TARGET_OBJECTS:performance_module>
    )
    target_link_libraries(game_engine PRIVATE Threads::Threads)
    target_include_directories(game_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Compiler-specific settings
if(MSVC)
    # Visual Studio specific settings
    add_compile_options(/W4)  # Warning level 4
    add_compile_options(/permissive-)  # Stricter C++ conformance
else()
    # GCC/Clang settings
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-parameter)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/scene)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/servers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/drivers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform)

# Optional: Unit tests
if(BUILD_TESTS)
    enable_testing()
    # Add test subdirectories here
endif()

# Optional: Documentation
if(BUILD_DOCS)
    # Add Doxygen documentation here
endif()

# Summary
message(STATUS "=== Build Configuration ===")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
